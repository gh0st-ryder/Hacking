class Solution {
    vector<int> vals;
    // first index: left; second index: right
    vector<vector<int>> dp;
    // [For debug]: store the optimal balloon to pick for this size
    vector<vector<int>> optimal;

    void debug(int left, int right) {
        // Base case: If no balloons are strictly between left and right
        if (left + 1 >= right) {            
            return;
        }
        int opt = optimal[left][right];
        printf("[%d, %d]: Pick balloon %d (val: %d) LAST\n", left, right, opt, vals[opt]);
        
        debug(left, opt);
        debug(opt, right);
    }

public:
    // O(n^2) time and space. 
    // This solution clears all test cases!
    // Bottom up DP.
    int maxCoins(vector<int>& nums_in) {
        int n = nums_in.size();

        // 1. Pad the array with 1s at both ends
        vals = vector<int>(n + 2, 1);
        for (int i = 0; i < n; i++) {
            vals[i + 1] = nums_in[i];
        }

        // DP table size is now (n + 2) x (n + 2)
        // Initializing with 0 automatically handles our base cases         
        dp = vector<vector<int>>(n + 2, vector<int>(n + 2, 0));
        optimal = vector<vector<int>>(n + 2, vector<int>(n + 2, 0));

        // 2. sz represents the distance between left and right (right - left)        
        for (int sz = 2; sz <= n + 1; sz++) {
            for (int left = 0; left <= n + 1 - sz; left++) {
                 int right = left + sz;
                 
                 // We are solving for the sub-problem solve(left, right) exclusive
                 int best = std::numeric_limits<int>::min();
                 int best_bi = -1;
                 
                 // 3. Pick the balloon bi to pop LAST in this range
                 // bi must be STRICTLY between left and right
                 for (int bi = left + 1; bi < right; bi++) {
                    
                    int my_val = vals[left] * vals[bi] * vals[right];
                    
                    my_val += dp[left][bi] + dp[bi][right];
                    
                    if (my_val >= best) {
                        best = my_val;
                        best_bi = bi;
                    }
                 }
                 dp[left][right] = best;
                 optimal[left][right] = best_bi;
            }
        }
        
        // Uncomment for debug
        // debug(0, n + 1);
        
        // Return the max coins for balloons strictly between 0 and n+1
        return dp[0][n + 1];
    }
};
