class Solution {
public:
    // O(n) space, O(n log n) time.
    void wiggleSort(vector<int>& nums) {
        vector<int> ns = nums;
        std::sort(ns.begin(), ns.end(), std::less<int>());
        int o_ptr=nums.size()-1;
        int e_ptr=nums.size()/2;
        if (nums.size() % 2 == 0) {e_ptr--;}
        for (int i=0; i<nums.size(); i++) {
            if (i % 2 == 0) {
                nums[i] = ns[e_ptr--];
            } else {
                nums[i] = ns[o_ptr--];
            }
        }
    }
};
