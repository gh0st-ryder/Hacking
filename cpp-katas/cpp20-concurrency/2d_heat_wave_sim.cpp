/*
 * The Scenario:
 * You are simulating heat spreading across a 1000x1000 metal plate. The grid
 * is represented by two 2D arrays: Current and Next. Every cell's new 
 * temperature is the average of its four immediate neighbors (North, South, 
 * East, West).
 * 
 * The Challenge (Domain Decomposition):
 * To keep the simulation fast, you cannot use a single mutex for the whole 
 * grid. Instead, you divide the 1000 rows among 100 worker threads. Each 
 * thread is responsible for exactly 10 rows. Because a cell's calculation 
 * depends on its neighbors, you must ensure that all threads finish 
 * calculating their assigned rows for Generation T before any thread begins
 *  calculating for Generation T+1.
 * 
 * Your Task:
 * Implement the GridSolver class.
*/

#include <vector>
#include <iostream>
#include <thread>
#include <barrier>
#include <functional>
#include <utility>
#include <syncstream>

class GridSolver {
    const int nworkers_=0, niters_=0;
    bool show_rows_=false;
    std::vector<std::vector<double>> grid_0_, grid_1_;
    using VVPtr = std::vector<std::vector<double>>*;
    VVPtr prev_=nullptr, next_=nullptr;
    std::vector<std::jthread> workers_;
    int cur_iter_=0;

    // A barrier for the workers to coordinate.
    using CompFunc = std::move_only_function<void() noexcept>;
    std::barrier<CompFunc> bar_;

    // This will purposely initialize next_, because the initial barrier wait will swap it to prev_.
    void initialize();

    void spawn_workers();

    void sync_func() noexcept {
        std::swap(prev_, next_);
        double sim_val=0;
        const int sz = grid_0_.size();
        for (int ri=0; ri<sz; ri++) {
            double row_val=0;
            for (int ci=0; ci<sz; ci++) {
                sim_val += (*prev_)[ri][ci];
                row_val += (*prev_)[ri][ci];
            }
            double row_avg = row_val / sz;
            if (show_rows_) {
                std::osyncstream(std::cout) << "Sync phase: Iter[" << cur_iter_ << "]" << " Row[" << ri << "] row_avg = " << row_avg << std::endl;
            }
        }
        double avg = sim_val / (sz*sz);
        std::osyncstream(std::cout) << "Sync phase: Iter[" << cur_iter_ << "] avg = " << avg << std::endl;
        cur_iter_++;
    }

  public:
    // TODO: Assumes that we will always have (sz % workers = 0).
    GridSolver(int sz, int workers, int iters, bool show_rows) :
      nworkers_(workers), niters_(iters), show_rows_(show_rows),
      grid_0_(sz, std::vector<double>(sz, 0)), grid_1_(sz, std::vector<double>(sz, 0)), 
      prev_(&grid_0_), next_(&grid_1_),
      bar_(workers, [this]() noexcept {sync_func();}) {
        initialize();
      }

    // Rule of 5.
    ~GridSolver() {
        // Force all the threads to finish first. This ensures the simulation has state available when needed.
        workers_.clear();
    }
    GridSolver(const GridSolver&) = delete;
    GridSolver& operator=(const GridSolver&) = delete;
    GridSolver(GridSolver&&) = delete;
    GridSolver& operator=(GridSolver&&) = delete;

    void simulate() {
        spawn_workers();
    }
};

void GridSolver::spawn_workers() {
    for (int i=0; i<nworkers_; i++) {
        workers_.push_back(std::jthread([this](int id) mutable {
            const int sz = grid_0_.size();
            const int nrows = sz / nworkers_;
            const int my_start = nrows * id;
            const int my_end = my_start + nrows-1;

            // Start them all at the same time.
            // std::osyncstream(std::cout) << "Starting worker thread " << id  << "; processing range ["  << my_start << ", " << my_end << "]."<< std::endl;
            bar_.arrive_and_wait();

            // Process each iteration
            for (int iter=0; iter < niters_; iter++) {
                // Do work for a single iteration
                // std::osyncstream(std::cout) << "Starting worker thread " << id  << " iteration " << iter << std::endl;
                for (int mri=my_start; mri<=my_end; mri++) {
                    for (int ci=0; ci<sz; ci++) {
                        std::vector<std::pair<int, int>> coords = {
                            {mri-1, ci}, {mri+1, ci},
                            {mri, ci-1}, {mri, ci+1},
                        };
                        double val = 0;
                        int cnt = 0;
                        for (const auto& [rr, cc] : coords) {
                            if (rr < 0 || rr >= sz || cc < 0 || cc >= sz) continue;
                            cnt++;
                            val += (*prev_)[rr][cc];
                        }
                        if (cnt) {
                            val /= cnt;
                            (*next_)[mri][ci] = val;
                        }
                    }
                }

                // Wait for all workers to sync, and let the completion func swap active work areas.
                // std::osyncstream(std::cout) << "Finished worker thread " << id  << " iteration " << iter << " waiting at the barrier."  << std::endl;
                bar_.arrive_and_wait();
                // std::osyncstream(std::cout) << "Finished worker thread " << id  << " iteration " << iter << " released from the barrier."  << std::endl;
            }            
        }, i));
    }
}

void GridSolver::initialize() {
    // Simulate a plate in the center of the grid that is super hot. 
    const int sz = grid_0_.size();
    const int chunksize = sz/nworkers_;
    const int midstart = nworkers_/2 * chunksize;

    for (int ri=midstart; ri<midstart+chunksize; ri++) {
        for (int ci=0; ci<sz; ci++) {
            (*next_)[ri][ci] = (*prev_)[ri][ci] = 1000.0;
        }
    }
    
}

int main() {

    // Please NOTE:
    // Uses the law of conservation of energy as a unit test!
    // Each iteration, the global average MUST remain the same, because this is a closed system heat simulation.
    
    // A smaller simulation, shown in full.
    // In this case, manually inspected that each iteration, the total average is ~200.
    std::cout << "--------------------------------------------------------" << std::endl;
    GridSolver gs(10, 5, 10, true);
    gs.simulate();

    // A larger simulation shown in brief.
    // In this case, manually inspected that each iteration, the total average is 10.
    std::cout << "--------------------------------------------------------" << std::endl;
    GridSolver gs2(1000, 100, 10, false);
    gs2.simulate();
}