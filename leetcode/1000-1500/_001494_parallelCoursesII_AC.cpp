
using U16 = unsigned short int;
class Solution {
    // 
    // We can represent each "state" of courses taken so far as a bitmask.
    //
    // Given the current "state", we can compute how many of the untaken courses can be taken next.
    // If this is <=k, we just take them, mark the new bitmask, and recurse
    // If this is > k, then we try each possible (n `choose` k) combination, and choose the best
    //
    // The result of these computations is stored in a cache dp.
    //
    // The recurrence equation is:
    // dp[mask] = dp[new_mask] + 1 => if <= k courses are now eligible
    //                                in this case new_mask is just mask | (new bits for the <k courses taken)
    // dp[mask] = min(dp[new_mask]) + 1 => if more than k courses are now eligible
    //                                in this case, we have a bunch of new_mask values
    //                                each new_mask is mask | (n `choose` k) new bits set
    // 

    int N=0;  // The total number of courses
    int K=0;  // The max courses we can take per semester, if prereqs are met.

    // To check for "eligibility" quickly, we can keep a bitmask of prereqs for each course
    // Key: course (0-indexed), Value: prereqs
    unordered_map<int, U16> prereqs; 

    // The DP cache.
    const int CACHE_SIZE = (1 << 15);
    vector<int> dp = vector<int>(CACHE_SIZE, -1);

    void compute_prereqs(const vector<vector<int>>& relations) {       
        for (int i=0; i<N; i++) {
            prereqs[i] = 0;
        } 
        for (const vector<int>& relation: relations) {
            int from = relation[0] - 1;
            int to = relation[1] - 1;
            prereqs[to] |= ((U16)1 << from);
        }
    }

    // Given a "mask" representing completed courses, is_eligible checks if course "check" can be completed next.
    bool is_eligible(int check, U16 mask) {
        return ((prereqs[check] & mask) == prereqs[check]);
    }

    // Given that "mask" courses are done (1 set in bit position per course done)
    // returns the new courses that can be done in the next iteration.
    vector<int> find_eligible(U16 mask) {
        vector<int> ans;
        for (int i=0; i<N; i++) {
            if (mask & ((U16)1 << i)) continue;  // already done
            if (is_eligible(i, mask)) ans.push_back(i);
        }
        return ans;
    }

    // Marks the new courses in eligible into mask and returns the new mask.
    U16 mark_eligible(U16 mask, const vector<int>& eligible) {
        for (int e : eligible) {
            mask |= ((U16)1 << e);
        }
        return mask;
    }

    // given m choices, we need to choose K of them
    // i keeps track of the current index while making the choices, so we can recurse.
    // leftover says how many more we have left to choose
    // 
    // Rewrote this function to use bitwise operations to avoid TLE.
    //
    vector<U16> m_choose_k(const vector<int>& choices, int i, int leftover) {
        if (i + leftover == choices.size()) {
            U16 new_mask = 0;
            for (int p=i; p<choices.size(); p++) {
                new_mask |= ((U16)1 << choices[p]);
            }            
            return {new_mask};
        }
        if (i == choices.size()) {
            printf("Called with C.size()=%d, %d, %d; Should not get here!", choices.size(), i, leftover);
            return {0};
        }
        vector<U16> ans;
        // Take index i
        {
            if (leftover == 1) {
                ans.push_back(((U16)1 << choices[i]));                
            } else {
                vector<U16> rec = m_choose_k(choices, i+1, leftover-1);
                for (U16 r : rec) {
                    r |= (((U16)1 << choices[i]));                    
                    ans.push_back(r);
                }
            }
        }
        // Drop index i
        {
            if (leftover == 0) {
                ans.push_back(0);
            } else {
                vector<U16> rec = m_choose_k(choices, i+1, leftover);
                for (U16 r : rec) {            
                    ans.push_back(r);
                }
            }
        }
        return ans;
    }

    // Given a "mask" representing finished courses, and total n courses, 
    // solve returns the number of semesters needed to finish all of them.
    int solve(U16 mask) {        
        // if (dp.find(mask) != dp.end()) return dp[mask];
        if (dp[mask] != -1) return dp[mask];
        int ans=std::numeric_limits<int>::max();

        vector<int> eligible = find_eligible(mask);
        if (eligible.size() <= K) {
            U16 new_mask = mark_eligible(mask, eligible);
            ans = solve(new_mask) + 1;
        } else {
            vector<U16> choices = m_choose_k(eligible, 0, K);
            for (const auto& new_mask : choices) {                
                int my_ans = solve(mask | new_mask) + 1;
                ans = std::min(ans, my_ans);
            }
        }

        // Insert into the cache
        dp[mask] = ans;
        return ans;
    }

  public:
    int minNumberOfSemesters(int n, vector<vector<int>>& relations, int k) {     
        N = n;   
        K = k;
        // memset(&dp[0], -1, sizeof(int) * CACHE_SIZE);
        compute_prereqs(relations);                   
        U16 done_mask = (1 << n) - 1;
        dp[done_mask] = 0;  // when all done, we need 0 more semesters.
        int ans = solve(0);   
        return ans;
    }
};
