class Solution {
    vector<int> nums;

    // findAnsNZ solves the problem assuming two things:
    // We solve for the original array between [left, right] only.
    // Assumes there are no zeros between [left, right].
    int findAnsNZ(int left, int right) {
        int count_neg=0;
        int first_neg=-1, last_neg=-1;
        for (int i=left; i<=right; i++) {
            if (nums[i] < 0) {
                count_neg += 1;
                if (first_neg == -1) first_neg = i;
                last_neg = i;
            }
        }        
        if ((count_neg % 2) == 0) return (right - left + 1);
        if (right == left) return 0;
        int choice_left = (last_neg-1) - left + 1;
        int choice_right = right - (first_neg+1) + 1;
        return std::max(choice_left, choice_right);
    }
public:
    int getMaxLen(vector<int>& nums_in) {
        nums = nums_in;

        int left=0;
        int best=0;
        for (int right=0; right<nums.size(); right++) {
            if (nums[right] == 0 || right == nums.size()-1) {
                
                if (nums[left] != 0) {
                    // first send out the prev selection for scanning
                    int pass_right = (nums[right] == 0 ? right-1 : right);
                    int my_ans = findAnsNZ(left, pass_right);
                    best = std::max(best, my_ans);
                }

                left = right+1;                 
            }
        }
        return best;
    }
};
