// Recurrence relation:
// minimax(left, right, k) = min (for all mid from left to right-k+1) {max(sum(left, mid), minimax(mid+1, right, k-1))}
// This solution has complexity O(n^2 * k)

// A better solution is using binary search on the output space!
// Consider M to be the max number in the array. We can't have a minimax value smaller than M.
// Consider SUM to be the sum of all the numbers in the array. There is no way the minimax value will be larger than SUM.
// This gives us a range [M, SUM] for the output minimax value. 
// We can binary search this output space!
//
// So we write a helper function as follows:
// Given a candidate m, it tells us whether we can divide up our array into k subarrays,
// where each subarray sums up to NO MORE than m.
// This is a bool function.
// We use this as a helper for the binary search.
class Solution {
    vector<int> nums;
    int K;

    // Uses a greedy strategy to accumulate contiguous subarrays initially.
    // However if at any point, the number of numbers leftover is equal to the number of subarrays needed, we exit out with true.
    bool canDivideIntoKofM(int left, int right, int m, int my_k) {               
        if (right - left + 1 == my_k) return true;  // assumes all individual nums are <= m
        if (right-left+1 < my_k) return false;
        if (my_k == 0) return false;
    
        int sum = 0;
        int index=left;
        
        for (; index <=right-my_k+1; index++) {
            // printf("left = %d, right=%d, my_k=%d, index=%d\n", left, right,my_k, index);
            sum += nums[index];
            if (sum > m) {  // Went too far
                return canDivideIntoKofM(index, right, m, my_k-1);
            }
        }
        return canDivideIntoKofM(index, right, m, my_k-1);
    }

    
    int binSearch(int minv, int maxv, int k) {
        if (minv == maxv) return minv;
        if (minv+1 == maxv) {
            if (canDivideIntoKofM(0, nums.size()-1, minv, k)) return minv;
            return maxv;
        }
        int mid = (minv+maxv)/2;
        if (canDivideIntoKofM(0, nums.size()-1, mid, k)) {
            return binSearch(minv, mid, k);
        }
        return binSearch(mid+1, maxv, k);
    }

public:
    int splitArray(vector<int>& nums_in, int k) {
        K=k;
        nums = nums_in;

        int sum=0, biggest=0;                
        for (int i=0; i<nums.size(); i++) {
            sum += nums[i];            
            biggest = std::max(biggest, nums[i]);
        }        

        // Initially the problem won't solve for minv, the problem will definitely solve for maxv;
        // printf("Trying\n");
        int ans = binSearch(biggest, sum, k);
        return ans;
    }
};
