class Solution {
public:
    int findMaxConsecutiveOnes(vector<int>& nums) {
        int best=0;
        bool flipped=false;
        int index_flipped=-1;
        int left=0; // the index where we start comparing (the start of the string of 1's, with flipping if needed)
        int right=0; // the next number to compare (1 after the end of the string of 1's, with flipping if needed)

        while (right < nums.size()) {
            while (right < nums.size() && (nums[right] == 1 || !flipped)) {
                if (nums[right] == 0) {
                    // first flip
                    flipped = true;
                    index_flipped = right;
                }
                right++;
            }

            // either came to the end or encountered a second zero
            int candidate = right - left;
            best = std::max(best, candidate);
            if (flipped) {
                flipped = false;
                left = index_flipped+1;
            }
        }
        return best;
    }
};
