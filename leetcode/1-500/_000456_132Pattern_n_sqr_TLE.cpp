class Solution {
    // We will first keep a hash table as follows (one linear scan):
    // Key: index i; Value: number that is the smallest to the left of index i
    unordered_map<int, int> least_left;

    // The we will traverse from the right to left
    // 
public:
    // 103/109 test cases passed; TLE rest
    // O(n^2) algorithm.
    bool oh_n_squared(vector<int>& nums) {
        int minleft = nums[0];
        for (int i=1; i<nums.size(); i++) {
            least_left[i] = minleft;
            minleft = std::min(minleft, nums[i]);
        }

        for (int i=1; i<nums.size()-1; i++) {
            // for [p, q, r] => p = minleft of current index, q = current index num, r = linear search
            for (int right=i+1; right<nums.size(); right++) {
                if (least_left[i] < nums[i] && nums[right] < nums[i] && nums[right] > least_left[i]) {
                    // printf("Found: %d, [i=%d]=>%d, [right=%d]=>%d\n", least_left[i], i, nums[i], right, nums[right]);
                    return true;
                }
            }
        }
        return false;
    }
    bool find132pattern(vector<int>& nums) {
        return oh_n_squared(nums);
    }
};
