class Solution {
    
public:
    int waysToSplitArray(vector<int>& nums) {
        vector<long long> prefix_sums(nums.size(), 0), suffix_sums(nums.size(), 0);
        long long sum=0;
        for (int i=0; i<nums.size(); i++) {
            prefix_sums[i] = sum;
            sum += nums[i];            
        }
        sum = 0;
        for (int i=nums.size()-1; i>=0; i--) {
            suffix_sums[i] = sum;
            sum += nums[i];
        }
        int count=0;
        for (int i=0; i<nums.size()-1; i++) {
            if (prefix_sums[i] + nums[i] >= suffix_sums[i]) {
                count++;
            }
        }
        return count;
    }
};
