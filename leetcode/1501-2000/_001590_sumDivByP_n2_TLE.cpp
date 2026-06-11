class Solution {
public:
    //
    // 144/145 cases pass; rest TLE
    // O(n^2) worst case performance
    //
    int minSubarray(vector<int>& nums, int p) {
        // For index i, the sum of all the nums that come before i.
        vector<long long> prefix_sums;
        long long total=0;
        for (int i=0; i<nums.size(); i++) {
            prefix_sums.push_back(total);
            total += nums[i];
        }
        if (total % p == 0) return 0;
        for (int sz=1; sz<nums.size(); sz++) {
            for (int right=nums.size()-1; right>=0; right--) {
                // Find the sum of [right-sz+1, right]
                int left = right-sz+1;
                if (left < 0) {continue;}
                long long subtract = prefix_sums[right] - prefix_sums[left] + nums[right];
                long long val = total - subtract;
                if ((val%p) == 0) return sz;
            }
        }
        return -1;
    }
};
