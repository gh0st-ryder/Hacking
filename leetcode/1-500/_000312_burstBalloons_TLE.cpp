class Solution {
    // This solution is functionally correct, but gets TLE after 31/73 test cases.
    vector<int> nums;
    unordered_map<string, int> cache;
public:

    // returns the index of the non-0 strictly to the right of right_from
    int findRightPos(const string& key, int right_from) {
        for (int f=right_from+1; f<key.size(); f++) {
            if (key[f] == '1') return f;
        }
        return -1;  // no right pos
    }

    int solve(string key, int unpopped) {
        // Check the cache.
        if (cache.find(key) != cache.end()) return cache[key];

        // Base case
        if (unpopped == 1) {
            for (int i=0; i<key.size(); i++) {
                if (key[i] == '1') {
                    cache[key] = nums[i];
                    return nums[i];
                }
            }
        }
        int best = std::numeric_limits<int>::min();
        // try each balloon that is not 0
        int l_val=1;
        for (int bi=0; bi<key.size() && bi != -1; ) {
            if (key[bi] == '0') {
                bi++;
                continue;
            }

            int r_ind = findRightPos(key, bi);
            int r_val = (r_ind == -1 ? 1 : nums[r_ind]);

            // Pop the balloon
            int my_points = l_val * nums[bi] * r_val;
            key[bi] = '0';

            // Solve recursively
            my_points += solve(key, unpopped-1);

            // Unpop the balloon
            key[bi] = '1';

            // Update l_val and bi.
            l_val = nums[bi];
            bi = r_ind;

            // Prepare my answer.
            best = std::max(best, my_points);
        }

        // Update the cache
        cache[key] = best;
        return best;
    }

    int maxCoins(vector<int>& nums_in) {
        nums = nums_in;
        string key(nums.size(), '1');
        int ans = solve(key, nums.size());
        return ans;
    }
};
