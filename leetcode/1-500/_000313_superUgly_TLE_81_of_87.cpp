class Solution {

    // Sort the primes
    // Start with the first number, save index into primes as primes_idx
    // Put that first number in the solution
    // Start forwards from the solution and loop through
    //    multiply it with primes from idx 0 to primes_idx
    //    find the smallest new number generated that is less than primes[primes_idx+1] put it in the solution
    //    if no such is found, add primes[primes_idx+1] to the solution, and increment primes_idx

    // Currently 81/87 test cases pass, rest are TLE.

public:
    int nthSuperUglyNumber(int n, vector<int>& primes) {
        if (n == 1) return 1;
        int count=1;
        std::sort(primes.begin(), primes.end(), std::less<int>());

        // Keep the solution as a vector and a set.
        std::vector<int> solution;
        std::unordered_set<int> sol_set;

        int primes_idx=0;
        solution.push_back(primes[0]);
        sol_set.insert(primes[0]);
        count++;

        // Optimization to avoid recomputing multiples
        // Key = Prime number, Value = the index within sol where the last multiple for this prime was a candidate
        std::unordered_map<int, int> sol_indices;

        while (count < n) {
            int candidate = -1; // The candidate, which is a multiple of a solution number and a prime            
            int can_prime_mult = 0;  // the prime number that generated this candidate
            int can_sol_index = 0;   // the solution index for this prime

            for (int pidx=0; pidx<=primes_idx; pidx++) {
                int start=0;
                if (sol_indices.find(primes[pidx]) != sol_indices.end()) {
                    start = sol_indices[primes[pidx]];
                }
                for (int i=start; i<solution.size(); i++) {
                    int check = solution[i] * primes[pidx];
                    if (sol_set.find(check) != sol_set.end()) continue;  // should be optimized away later.
                    // Found the smallest candidate for this primes multiplier
                    if (candidate == -1 || candidate > check) {
                        candidate = check;
                        can_prime_mult = primes[pidx];
                        can_sol_index = i;
                    }
                    break;
                }
            }
            if (primes_idx < primes.size()-1 && candidate > primes[primes_idx+1]) {
                solution.push_back(primes[primes_idx+1]);
                sol_set.insert(primes[primes_idx+1]);
                primes_idx += 1;
                // printf("Adding primes: %d\n", primes[primes_idx]);
            } else {
                solution.push_back(candidate);
                sol_set.insert(candidate);
                sol_indices[can_prime_mult] = can_sol_index + 1;
                // printf("Adding candidate: %d\n", candidate);
            }
            count++;
        }
        return solution.back();
    }
};
