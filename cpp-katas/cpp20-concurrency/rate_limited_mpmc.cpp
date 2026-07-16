/*
 * The Scenario:
 * You are writing a backend dispatcher. You have multiple producer threads
 * generating tasks and multiple consumer threads executing them (Standard
 * Multi-Producer, Multi-Consumer).
 * 
 * The Catch:
 * While your consumer thread pool might have 10 active threads pulling from
 * the queue, the tasks themselves involve querying an external hardware 
 * resource (like an inference accelerator) that strictly supports a maximum 
 * of 3 concurrent requests. If a 4th consumer thread tries to execute a task, 
 * it must block until one of the other 3 finishes interacting with the 
 * hardware, even if the consumer has already pulled the task off the queue.
 * 
 * Your Task:
 * Implement a RateLimitedDispatcher class.
*/

#include <vector>
#include <mutex>
#include <condition_variable>
#include <semaphore>
#include <thread>
#include <barrier>
#include <syncstream>
#include <random>
#include <iostream>
#include <string>
#include <unordered_map>

// Arguments to the RateLimitedDispatcher are bundled here.
struct RLArgs {
    int q_size;
    int items;
    int producers, consumers; 
    int p_min_t, p_max_t, c_min_t, c_max_t;
};

template<int R>
class RateLimitedDispatcher {
    //////////////////////////////////////////////////////////////////////////////
    //               Simulation state is tracked here.
    //////////////////////////////////////////////////////////////////////////////

    // Controls access to all the shared state, and associated bookkeeping.
    mutable std::mutex mut_;

    // This buffer tracks the actual items to consume.
    std::vector<int> buffer_;

    // The next places to read from or write to in buffer_, subject to occupancy_.
    int read_ptr_=0, write_ptr_=0;
    // Keeps track of how many items are in the buffer
    // If 0, then read_ptr_ is invalid.
    // If buffer_.size(), then write_ptr_ is invalid.
    int occupancy_=0;

    // How many producers and consumers do we have.
    const int producers_=0, consumers_=0;
    std::vector<std::jthread> prods_, cons_;
    // The production and consumption time simulations in milliseconds.
    const int p_min_t_=0, p_max_t_=0, c_min_t_=0, c_max_t_=0;
    // Make sure all threads start together.
    std::barrier<> bar_;
    
    // Incremented by the each of the producers when they finish all production tasks.
    int done_ = 0;
    // How many items will be produced/consumed in total.
    // cur_items_ is used by producers to give each item a seq number serially.
    const int items_=0;
    int cur_items_=0;

    // Consumers will wait for producers to signal await_prod.
    // Producers will wait for consumers to signal await_cons.
    std::condition_variable await_prod_, await_cons_;

    // This counting semaphore is used to restrict access to the rate limited hardware.
    std::counting_semaphore<R> sem_;

    //////////////////////////////////////////////////////////////////////////////
    //               Validation state is tracked here.
    //////////////////////////////////////////////////////////////////////////////

    // Key: Item seq number, Value: Number of times it was consumed.
    std::unordered_map<int, int> items_val_;

    //////////////////////////////////////////////////////////////////////////////
    //               Internal APIs go here.
    //////////////////////////////////////////////////////////////////////////////

    void run_producers();
    void run_consumers();

    // Called by the producer and consumer respectively.
    bool generate_item(int&);
    bool consume_item(int&);

    // Validation phase for the experiment.
    bool validate() {
        
        while (true) {
            bool spin = false;
            {
                std::scoped_lock l(mut_);
                spin = done_ != producers_ || occupancy_ != 0;
            }            
            if (!spin) break;
            std::this_thread::sleep_for(std::chrono::seconds(3));            
        }        

        // Let's ensure all producers and consumers are actually done (join() the threads).
        prods_.clear();
        cons_.clear();

        for (int i=0; i<items_; i++) {
            if (items_val_[i] != 1) {
                std::cout << "Validation failed: key " << i << " has value " << items_val_[i] << std::endl;
                return false;
            }
        }
        return true;
    }

  public:
    // q_size is the size of the internal buffer.    
    RateLimitedDispatcher(RLArgs rl) 
      : buffer_(rl.q_size, 0), items_(rl.items), producers_(rl.producers), consumers_(rl.consumers),
        p_min_t_(rl.p_min_t), p_max_t_(rl.p_max_t), c_min_t_(rl.c_min_t), c_max_t_(rl.c_max_t), bar_(rl.producers),
        sem_(R) {}
    
    // Returns true if the simulation succeeds the validation phase.
    bool simulate() {
        run_consumers();
        run_producers();
        return validate();
    }
    
    

    // Rule of 5.
    ~RateLimitedDispatcher() = default;
    // Note Copyable or Moveable.
    RateLimitedDispatcher(const RateLimitedDispatcher&) = delete;
    RateLimitedDispatcher& operator=(const RateLimitedDispatcher&) = delete;
    RateLimitedDispatcher(RateLimitedDispatcher&&) = delete;
    RateLimitedDispatcher& operator=(RateLimitedDispatcher&&) = delete;
};

// called by producer, returns false when all production is done.
template<int R>
bool RateLimitedDispatcher<R>::generate_item(int& my_item) {
    std::unique_lock l(mut_);

    // This is the seq number of the next item to generate.
    my_item = cur_items_;
    if (my_item == items_) {
        return false;
    }
    // Increment count for the next producer.
    cur_items_++;

    // First check if this producer has space.
    while (occupancy_ == buffer_.size()) {
        await_cons_.wait(l);
    }

    // Place the item.
    buffer_[write_ptr_] = my_item;
    write_ptr_ += 1;
    write_ptr_ %= buffer_.size();
    occupancy_ += 1;

    // If needed signal the waiting consumers.
    if (occupancy_ == 1) {
        await_prod_.notify_all();
    }
    return true;
}

// called by consumer, returns false when all production is done.
// Note that the consumer is allowed to pull REGARDLESS of rate limiting.
// The rate limiting is done later, after the pull.
template<int R>
bool RateLimitedDispatcher<R>::consume_item(int& my_item) {
    std::unique_lock l(mut_);

    await_prod_.wait(l, [this] {
        return (occupancy_ > 0) || (done_ == producers_);
    });

    // Check if production and consumption is done.
    if (done_ == producers_ && occupancy_ == 0) {
        return false;
    }

    // Extract the item; the consumer is allowed to pull it regardless of the rate limiting
    my_item = buffer_[read_ptr_];
    read_ptr_ += 1;
    read_ptr_ %= buffer_.size();
    occupancy_ -= 1;

    // Signal any waiting producers if needed.
    if (occupancy_ + 1 == buffer_.size()) {
        await_cons_.notify_all();
    }
    return true;
}

template<int R>
void RateLimitedDispatcher<R>::run_producers() {
    
    for (int i=0; i<producers_; i++) {
        prods_.push_back(std::jthread([this, i]() mutable {
            // Let's start all the producers together.            
            bar_.arrive_and_wait();            
            std::osyncstream(std::cout) << "Producer[" << i << "] starts producing." << std::endl;

            while (1) {                
                // Sleep time to simulate random production time for the item
                std::random_device rd;
                std::mt19937 g(rd());
                std::uniform_int_distribution<int> dist(p_min_t_, p_max_t_);
                int rng = dist(g);

                int my_item = 0;
                if (!generate_item(my_item)) {
                    std::osyncstream(std::cout) << "Producer[" << i << "] done producing." << std::endl;
                    break;
                }                

                std::this_thread::sleep_for(std::chrono::milliseconds(rng));
                std::osyncstream(std::cout) << "Producer[" << i << "] generated item I" << my_item << " taking " << rng << " millisec." << std::endl;
                 
            }            
            {
                std::scoped_lock l(mut_);
                done_ += 1;
                await_cons_.notify_all();
            }            
        }));
    }

}

template<int R>
void RateLimitedDispatcher<R>::run_consumers() {    
    for (int i=0; i<consumers_; i++) {
        cons_.push_back(std::jthread([this, i]() mutable {
            std::osyncstream(std::cout) << "Consumer[" << i << "] starts consuming." << std::endl;

            while (1) {
                int my_item = 0;
                if (!consume_item(my_item)) {
                    std::osyncstream(std::cout) << "Consumer[" << i << "] done consuming." << std::endl;
                    break;                    
                }

                // Rate limit the consumer due to the hardware constraint on resourcing.
                sem_.acquire();
                std::osyncstream(std::cout) << "Consumer[" << i << "] acquired a hardware slot." << std::endl;               

                // Sleep to simulate random consumption time for the item
                std::random_device rd;
                std::mt19937 g(rd());
                std::uniform_int_distribution<int> dist(c_min_t_, c_max_t_);
                int rng = dist(g);
                
                std::osyncstream(std::cout) << "Consumer[" << i << "] consuming item I" << my_item << " taking " << rng << " millisec" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(rng)); 

                std::osyncstream(std::cout) << "Consumer[" << i << "] releasing a hardware slot." << std::endl;
                sem_.release();
                {
                    std::scoped_lock l(mut_);
                    items_val_[my_item] += 1;
                }
            }
        }));
    }
}

void run_test_case(const std::string& name, RLArgs args, int& launched, int& passed) {
    launched++;
    std::cout << "----------------------------------------------------------------------------------" << std::endl;
    std::cout << "Starting test case: " << name << std::endl;
    std::cout << "----------------------------------------------------------------------------------" << std::endl;
    RateLimitedDispatcher<3> rld(args);
    bool val = rld.simulate();
    std::cout << "----------------------------------------------------------------------------------" << std::endl;
    std::cout << "Result: " << (val ? "PASSED" : "FAILED") << std::endl;
    std::cout << "----------------------------------------------------------------------------------" << std::endl;
    if (val) passed++;
}

int main() {    
    int launched=0, passed=0;
    // buf_size, num_items, num_producers, num_consumers, {min, max} times for production, {min, max} times for consumption.
    
    // Just a standard test case.
    run_test_case("Standard", {10, 20, 5, 5, 100, 500, 100, 500}, launched, passed);

    // Just a standard mid sized test case.
    run_test_case("Standard-mid", {20, 100, 15, 15, 100, 500, 100, 500}, launched, passed);

    // Just a standard mid sized test case, with slow consumers.
    run_test_case("Standard-mid-slow-cons", {20, 100, 15, 15, 100, 500, 500, 900}, launched, passed);

    // Just a standard mid sized test case with slow producers.
    run_test_case("Standard-mid-slow-prod", {20, 100, 15, 15, 500, 900, 100, 500}, launched, passed);

    // Just a standard mid sized test case, with fewer consumers and many more producers.
    run_test_case("Standard-mid-lots-prod-few-cons", {20, 100, 30, 5, 100, 500, 100, 500}, launched, passed);

    // Just a standard mid sized test case, with fewer producers and lots of consumers.
    run_test_case("Standard-mid-few-prod-lots-cons", {20, 100, 5, 30, 100, 500, 100, 500}, launched, passed);

    // Just a standard mid sized test case, with a tiny ass buffer.
    run_test_case("Standard-mid", {3, 100, 15, 15, 100, 500, 100, 500}, launched, passed);

    // TODO: Need to check that the hardware constraint on no more than 3 also holds at all times.

    std::cout << "Overall result: " << passed << "/" << launched << " cases passed." << std::endl;
}