class Solution {
    vector<int> ns_;  // the actual numbers.
    unordered_set<int> explored_;  // stores indices

    bool matchesDirection(int val, bool forward) {
        return ((val > 0 && forward) || (val < 0 && !forward));
    }    
    
    bool exploreForCycle(int index, const bool forward) {
        unordered_set<int> touched;  // stores indices

        while (true) {
            if (!matchesDirection(ns_[index], forward)) {
                return false;  // Direction change before we found a cycle
            }
            if (touched.find(index) != touched.end()) {
                // Found a cycle!
                printf("Found cycle at index %d\n", index);
                return true;
            }
            touched.insert(index);
            explored_.insert(index);
            int old_index = index;
            index += ns_[index];  // move forward or backward as needed
            index += ns_.size();
            index %= ns_.size();
            if (index == old_index) {
                // Ignore size 1 cycle.
                return false;
            }
        }
    }
public:
    // O(n) space, O(n) time.
    bool circularArrayLoop(vector<int>& nums) {
        ns_ = nums;
        for (int i=0; i<ns_.size(); i++) {
            if (explored_.find(i) != explored_.end()) {
                continue;
            }
            if (exploreForCycle(i, ns_[i] > 0)) {
                return true;
            }
        }
        return false;
    }
};
