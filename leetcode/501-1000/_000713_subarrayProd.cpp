class Solution {

    // Finds the number of contiguous subarrays in [left, right] both inclusive.
    // This is the sum of all natural nums from 1 to (right-left+1)
    // This also excludes the contiguous subarrays in [left, stop] since those were counted before.
    int numContiguous(int left, int right, int stop) {
        int n = right-left+1;
        int count = (n*(n+1))/2;
        int exclude = 0;
        if (stop != -1 && stop >= left) {
            int m = stop-left+1;
            exclude = (m*(m+1))/2;
        }
        int new_count = count-exclude;        
        return new_count;
    }
public:
    int numSubarrayProductLessThanK(vector<int>& nums, int k) {
        int total=0;
        int left=0;  // the start of our interval (inclusive)
        int right=0; // the number on the right to examine
        int stop=-1;
        long long prod=1;  // the running product

        while (right < nums.size() && left < nums.size()) {
            // Skip all numbers that are straight up greater than or equal to k
            while (right < nums.size() && nums[right] >= k) {
                right++;
                left = right;
                prod=1;                
            }            
            // Advance right until prod >= k
            while (right < nums.size() && prod*nums[right] < k) {
                prod *= nums[right];
                right++;                
            }            
            total += numContiguous(left, right-1, stop);
            // Advance left            
            if (nums[left]) prod /= nums[left];
            left++;            
            // Reset stop.
            stop = right-1;
        }
        return total;
    }
};
