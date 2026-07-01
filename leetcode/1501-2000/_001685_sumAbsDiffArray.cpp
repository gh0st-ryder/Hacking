class Solution {
    
    // For any index i
    // All numbers at index j < i are less than or equal to nums[i]
    // All numbers at index j > 1 are greater than or equal to nums[i]
    //
    // We can find the prefix sum of the array
    // Then for all nums before the index i, the sum of absolute differences is 
    // i * nums[i] - prefix_sum[i] + nums[i]
    // And for all nums after the index i, the sum of absolute difference is
    // (prefix_sum[nums.size()-1] - prefix_sum[i]) - (nums.size() - 1 - i) * nums[i]

public:
    // O(n) runtime.
    vector<int> getSumAbsoluteDifferences(vector<int>& nums) {
        vector<int> psum;  // up to and including the index
        int sum=0;
        for (int n : nums) {
            sum += n;
            psum.push_back(sum);
        }

        vector<int> ans;
        for (int i=0; i<nums.size(); i++) {
            int before = i * nums[i] - psum[i] + nums[i];
            int after = psum[nums.size()-1] - psum[i] - (nums.size()-1-i) * nums[i];           
            int total = before + after;
            ans.push_back(total);
        }
        return ans;
    }
};
