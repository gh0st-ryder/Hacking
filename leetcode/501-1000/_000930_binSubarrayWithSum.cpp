class Solution {
public:

    int numSubarraysWithSum(vector<int>& nums, int goal) {
        // Key = prefix_sum_value, Value = freq of times this prefix_sum was encountered
        unordered_map<int, int> prefix_sums;
        int sum=0;
        int count=0;
        prefix_sums[0] = 1;  // empty array, so we match this part.
        for (int n : nums) {
            sum += n;

            int check = sum - goal;
            if (prefix_sums.find(check) != prefix_sums.end()) {
                count += prefix_sums[check];
            }
            prefix_sums[sum]++;
        }
        return count;
    }
};
