class Solution {
    // Binary search on the output space strikes once again!
    // 
    // Write a function to determine: 
    // Can I place the m balls such that there is "at least" x distance between each consecutive balls
    // 
    // Then run binary search for x going from [1, max-min+1]

    bool canPlaceX(const vector<int>& position, int m, int x) {        
        int last = -1;
        int count=0;
        for (int ptr=0; ptr<position.size(); ptr++) {
            if (last == -1 || (position[ptr] - position[last] >= x)) {
                count++;
                last=ptr;
                if (count == m) break;
            }            
        }
        return count == m;
    }

    int binSearch(const vector<int>& position, int m, int left, int right) {
        if (left == right) return left;
        if (left + 1 == right) {
            if (canPlaceX(position, m, right)) return right;
            return left;
        }
        int mid = (left+right)/2;
        if (canPlaceX(position, m, mid)) {
            return binSearch(position, m, mid, right);
        }
        return binSearch(position, m, left, mid);
    }
public:
    // O(n log n)
    // Sorting is n log n
    // The bin search is log n, and work done per scan is n => therefore total n log n
    int maxDistance(vector<int>& position, int m) {        
        std::sort(position.begin(), position.end());
        int minv=std::numeric_limits<int>::max();
        int maxv=std::numeric_limits<int>::min();
        for (int n : position) {
            minv = std::min(minv, n);
            maxv = std::max(maxv, n);
        }
        return binSearch(position, m, 1, maxv-minv+1);
    }
};
