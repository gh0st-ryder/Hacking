struct Range {
    // Both are indices below.
    int left=0;   // inclusive
    int right=0;  // inclusive
    bool operator==(const Range& range) const {
        return left == range.left && right == range.right;
    }
    bool operator<(const Range& rhs) const {
        if (left != rhs.left) return left < rhs.left;
        return right < rhs.right;  // smaller piece will be stuck before the current bigger piece with the same left index value.
    }
};

// This solution is based on a scanline algorithm essentially.
// Simulate a scanline parallel to the x-axis, that starts from the bottom and moves upward.
// We start scanning from the smallest height to the largest.
// We break up the ranges as needed.
// Before each break, we compute any new best.
//
// This solution is functionally correct, and also clears all test cases.
// The fix was to use std::set::upper_bound() to search within the set within log n time, instead of a linear scan of O(n) each time.
class Solution {
    // Stores the original heights
    vector<int> heights;

    // Key: single height; Value: All the indices with that height
    unordered_map<int, set<int>> height_to_indices;

    // Stores the ranges we are considering.
    set<Range> ranges;

    // Stores the heights (only unique values).
    priority_queue<int, vector<int>, std::greater<int>> mh;

    void process_height(int next_ht, const set<int>& htoi, int& best) {    
        if (ranges.empty()) return;  // this should not happen

        for (int index : htoi) {            
            // First get the iterator the one past what we want.
            auto range_it = ranges.upper_bound({index, INT_MAX});
            if (range_it == ranges.begin()) continue;  // this should not happen

            // Find the next range to process
            --range_it;            
            Range range = *range_it;                
            if (range.left <= index && range.right >= index) {                    
                // Process it first
                int my_sz = range.right - range.left + 1;
                int cand = my_sz * next_ht;
                best = std::max(best, cand);                    

                // Then add the two new pieces.
                if (range.left != range.right) {
                    if (range.left <= index-1) ranges.insert({range.left, index-1});    // safe
                    if (range.right >= index+1) ranges.insert({index+1, range.right});   // safe
                }

                // Then delete the original big piece.
                range_it = ranges.erase(range_it);  // safe                
            }                
            
        }
    }

public:
    int largestRectangleArea(vector<int>& heights_in) {
        heights = heights_in;

        for (int i=0; i<heights.size(); i++) {
            height_to_indices[heights[i]].insert(i);
        }

        // Initially we just have a single range.
        ranges.insert({0, (int)heights.size()-1});

        // Populate the heap.
        for (const auto& kv : height_to_indices) {
            mh.push(kv.first);
        }

        // This is the best rectangle we have found so far.
        int best = 0;

        while (!mh.empty()) {
            // Find the next height to process.            
            int next_ht = mh.top(); mh.pop();

            // Find all the indices with this height.
            const set<int>& htoi = height_to_indices[next_ht];

            // Break up the prev ranges and simultaneously adjust our best if needed.
            process_height(next_ht, htoi, best);
        }

        return best;
    }
};
