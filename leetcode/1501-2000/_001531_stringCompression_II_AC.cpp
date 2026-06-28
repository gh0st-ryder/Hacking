class Solution {
    struct Info {
        int val;  
        char ch;  
        int num;    
    };

    // Caveat: This isn't strictly DP anymore, it is beam search now.
    // We kept a vector<Info> instead of an Info for the dp cell
    // However we prune it (hence beam search)
    //
    // 3D DP: dp[index][ki] stores a vector of non-dominated Info states
    vector<vector<vector<Info>>> dp;

    // Pruning logic: Only remove if we have an objectively worse state
    void add_and_prune(vector<Info>& current_states, const Info& new_info) {
        for (const auto& existing : current_states) {
            // If they are identical in 'ch' and 'num', we only keep the one with lower val
            if (existing.ch == new_info.ch && existing.num == new_info.num) {
                if (existing.val <= new_info.val) return; // New one is worse/duplicate
            }
        }

        // Remove old states that are strictly worse than the new one
        for (auto it = current_states.begin(); it != current_states.end(); ) {
            if (it->ch == new_info.ch && it->num == new_info.num && new_info.val < it->val) {
                it = current_states.erase(it);
            } else {
                ++it;
            }
        }

        current_states.push_back(new_info);
    }

public:
    int getLengthOfOptimalCompression(string s, int k) {
        int n = s.size();
        dp = vector<vector<vector<Info>>>(n + 1, vector<vector<Info>>(k + 1));

        // Base case: index = n, length is 0, empty char, 0 count
        for (int ki = 0; ki <= k; ki++) {
            dp[n][ki].push_back({0, ' ', 0});
        }

        // DP Case: Fill table backwards
        for (int index = n - 1; index >= 0; index--) {
            for (int ki = 0; ki <= k; ki++) {
                
                // Process keep
                for (const auto& rec : dp[index + 1][ki]) {
                    Info keep;
                    keep.ch = s[index];
                    keep.num = (s[index] == rec.ch) ? rec.num + 1 : 1;
                    
                    auto get_len = [](int n) {
                        if (n == 0) return 0;
                        if (n == 1) return 1;
                        if (n < 10) return 2;
                        if (n < 100) return 3;
                        return 4;
                    };
                    
                    int delta = (s[index] == rec.ch) ? (get_len(keep.num) - get_len(rec.num)) : 1;
                    keep.val = rec.val + delta;

                    add_and_prune(dp[index][ki], keep);
                }

                // Process drop
                if (ki > 0) {
                    for (const auto& drop : dp[index + 1][ki - 1]) {
                        add_and_prune(dp[index][ki], drop);
                    }
                }
            }
        }

        int ans = 10000;
        for (const auto& res : dp[0][k]) {
            ans = min(ans, res.val);
        }
        return ans;
    }
};
