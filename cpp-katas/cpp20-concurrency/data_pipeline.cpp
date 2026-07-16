/*
 * Problem Statement: 
 * The Scenario: You are building an execution engine that processes requests 
 * in three distinct phases: Fetch, Process, and Aggregate.
 * 
 * Fetch: A master thread generates N distinct data payloads (represented as 
 * integers).
 * Process: N independent worker threads take a payload, perform a long-
 * running computation on it (e.g., multiplying by 2), and return the result.
 * Aggregate: A final thread takes all N processed results and sums them up.
 * 
 * The Catch: The Aggregator thread must be spawned at the very beginning 
 * alongside the workers, but it cannot begin aggregating until all worker 
 * threads have finished their processing. 
 * Furthermore, the workers need to cleanly pass their processed results 
 * to the Aggregator asynchronously without using a shared std::vector 
 * protected by a mutex.
 * 
 * Your Task:Implement the Pipeline class.
*/

#include <future>
#include <vector>
#include <thread>
#include <latch>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <iostream>
#include <syncstream>

class Pipeline {
    int N_=0;
    std::latch launch_, compute_, done_;
    std::vector<int> payload_;
    std::vector<std::future<int>> data_;
    std::vector<std::jthread> threads_;
    std::jthread aggr_;
    int ans_=0;

    // The three phases.
    void fetch() {        
        for (int i=1; i<=N_; i++) {
            payload_.push_back(i);
        }

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(payload_.begin(), payload_.end(), g);
    }

    void processAndAggregate() {
        

        // Start the aggregator first;
        aggr_ = std::jthread([this]() {
            // wait for the compute phase to start
            std::osyncstream(std::cout) << "    Aggr thread awaiting compute " << std::endl;
            this->compute_.arrive_and_wait();
            std::osyncstream(std::cout) << "    Aggr thread computing " << std::endl;

            ans_ = 0;            
            for (auto& f : data_) {
                int val = f.get();
                ans_ += val;
            }
            std::osyncstream(std::cout) << "    Aggr thread finished " << std::endl;
            this->done_.arrive_and_wait();
        });

        // Start the N_ threads for workers.
        for (int i=0; i<N_; i++) {
            int arg = payload_[i];
            std::promise<int> prom;
            data_.push_back(prom.get_future());

            threads_.push_back(std::jthread([this, prom=std::move(prom)](int arg, int seq) mutable {
                // wait until everything is ready to go
                std::osyncstream(std::cout) << "    Thread " << seq << " awaiting launch" << std::endl;
                this->launch_.arrive_and_wait();
                std::osyncstream(std::cout) << "    Thread " << seq << " computing " << std::endl;

                // Perform the long running calculation.
                int ans = arg * 2;
                int tt = rand() % 5;
                std::this_thread::sleep_for(std::chrono::seconds(tt));
                prom.set_value(ans);

                // Synchronize for the computation.
                std::osyncstream(std::cout) << "    Thread " << seq << " finished " << std::endl;
                this->compute_.arrive_and_wait();
                
            }, payload_[i], i));
        }

    }

  public:
    explicit Pipeline(int n) : N_(n), launch_(N_), compute_(N_+1), done_(2) {}

    // Rule of 5.
    ~Pipeline() = default;
    Pipeline(const Pipeline&) = delete;
    Pipeline& operator=(const Pipeline&) = delete;
    Pipeline(Pipeline&&) noexcept = delete;
    Pipeline& operator=(Pipeline&&) noexcept = delete;

    int runPipeline() {        
        fetch();        
        processAndAggregate();

        // The main thread will wait to get the answer too.
        done_.arrive_and_wait();

        std::osyncstream(std::cout) << "Final answer = " << ans_ << std::endl;        
        return ans_;
    }
};

// Generates a test case for n first natural numbers, and checks the answer.
void testCase(int n) {
    std::cout << "Starting TestCase for Val= " << n << std::endl;
    Pipeline p(n);
    int sumn = (n+1) * n / 2;
    int want = 2 * sumn;
    int got = p.runPipeline();
    if (want == got) {
        std::cout << "[TestCase Val=" << n << "]: PASSED!" << std::endl;
    } else {
        std::cout << "[TestCase Val=" << n << "]: FAILED... got " << got << " want " << want << std::endl;
    }
}

int main() {
    testCase(5);
    testCase(10);
}



