class Solution {
    // Key: num; Value: freq of num
    unordered_map<int, int> freq;
public:
    int maxOperations(vector<int>& nums, int k) {
        int operations=0;
        for (int n : nums) {
            freq[n] += 1;
        }
        for (int i=0; i<nums.size(); i++) {
            if (freq[nums[i]] == 0) continue;

            int search = k - nums[i];
            if (search == nums[i]) {
                // Look for doubles
                if (freq[nums[i]] < 2) continue;
                freq[nums[i]] -= 2;
                operations++;
            } else {
                if (freq[search] == 0) continue;
                freq[nums[i]] -= 1;
                freq[search] -= 1;
                operations++;
            }
        }
        return operations;
    }
};
