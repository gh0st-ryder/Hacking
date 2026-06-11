class Solution {
public:
    vector<int> pivotArray(vector<int>& nums, int pivot) {
        vector<int> ans = nums;
        // count counts the number of elems equal to pivot
        int count=0;

        // left will store the next number less than pivot
        // right will store the next number greater than pivot
        int left=0, right=nums.size()-1;

        for (int i=0; i<nums.size(); i++) {
            if (nums[i] < pivot) {                
                ans[left] = nums[i];
                left++;
            } else if (nums[i] > pivot) {
                ans[right] = nums[i];
                right--;
            } else {
                count++;                
            }
        }
        if (right < nums.size()-1) {
            std::reverse(ans.begin()+right+1, ans.end());
        }
        for (int i=left; i<=right; i++) {
            ans[i] = pivot;
        }
        return ans;
    }
};
