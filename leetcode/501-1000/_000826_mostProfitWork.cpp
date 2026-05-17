class Solution {
    struct Job {
        int difficulty=0;
        int profit=0;
    };
public:
    int maxProfitAssignment(vector<int>& difficulty, vector<int>& profit, vector<int>& worker) {
        vector<Job> jobs;
        for (int i=0; i<difficulty.size(); i++) {
            jobs.push_back({difficulty[i], profit[i]});
        }
        std::sort(jobs.begin(), jobs.end(), [](const Job& lhs, const Job& rhs) -> bool {
            if (lhs.profit != rhs.profit) {
                return lhs.profit > rhs.profit;  // first by ascending order of profit.
            }
            return lhs.difficulty < rhs.difficulty;  // then by descending order of difficulty.
        });

        // Sort by most capable worker first.
        std::sort(worker.begin(), worker.end(), std::greater<int>());

        int job_ptr=0, worker_ptr=0;
        int total=0;
        // Start assigning jobs to workers. Prefer assigning the most profit job, until the worker can't handle it due to difficulty.
        while (job_ptr < jobs.size() && worker_ptr < worker.size()) {
            Job curr_job = jobs[job_ptr];

            if (curr_job.difficulty <= worker[worker_ptr]) {
                total += curr_job.profit;
                worker_ptr++;  // next worker
            } else {
                job_ptr++;  // try next job.
            }
        }
        return total;
    }
};
