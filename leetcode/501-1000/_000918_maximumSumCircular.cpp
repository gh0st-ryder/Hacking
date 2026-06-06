class Solution {
public:
    int maxSubarraySumCircular(vector<int>& ns) {        
        vector<int> nums = ns;

        // First find the best using Kadane's algorithm.
        int sum=nums[0];
        int best=nums[0];

        for (int i=1; i<nums.size(); i++) {
            sum = std::max(sum + nums[i], nums[i]);
            best = std::max(best, sum);
        }

        // Find the prefix and suffix sums.
        unordered_map<int, int> prefix_sums, suffix_sums;
        sum = 0;        
        for (int i=0; i<nums.size(); i++) {
            prefix_sums[i] = sum;
            sum += nums[i];
        }
        sum=0;
        for (int i=nums.size()-1; i>=0; i--) {
            suffix_sums[i] = sum;
            sum += nums[i];
        }
        
        // Find the best suffix sum to my right
        unordered_map<int, int> best_suffix_right;
        int best_right=0;
        for (int i=nums.size()-1; i>=0; i--) {
            best_suffix_right[i] = best_right;
            best_right = std::max(suffix_sums[i], best_right);
        }

        // Now adjust the best for wraparound
        for (int i=0; i<nums.size(); i++) {            
            int wrap_sum = prefix_sums[i] + nums[i] + best_suffix_right[i];
            best = std::max(best, wrap_sum);
        }
        return best;
    }
};
