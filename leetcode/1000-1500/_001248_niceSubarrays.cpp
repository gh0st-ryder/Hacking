class Solution {    
    using MinHeap = std::priority_queue<int, vector<int>, std::greater<int>>;
    // MinHeap of indices of odd numbers.
    MinHeap mh;
    
    int getContributions(int left) {
        if (mh.empty()) return 0;
        if (left > mh.top()) return 0;  // This should not happen
        return mh.top() - left + 1;
    }
public:
    int numberOfSubarrays(vector<int>& nums, int k) {
        // left is the beginning of my window
        // right is the last index I examined
        int left=0, right=0;
        if (nums[left]%2) mh.push(left);

        int count=(k == 1 && mh.size()==1 ? 1 : 0);
        for (int right=1; right<nums.size(); right++) {
            if (nums[right]%2) {  
                // right is an odd number
                if (mh.size() < k) {  
                    // just push right and count if needed
                    mh.push(right);                                        
                    if (mh.size() == k) {
                        count += getContributions(left);
                    }
                } else {  
                    // I need to drop the leftmost index and advance the left pointer
                    mh.push(right);
                    int new_left = mh.top()+1;
                    mh.pop();
                    left = new_left;                    
                    count += getContributions(left);
                }
            } else {  
                // right is an even number, just count the contributions if needed
                if (mh.size() == k) {
                    count += getContributions(left);
                }
            }
        }
        return count;
    }
};
