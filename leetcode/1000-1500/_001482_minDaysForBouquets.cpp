class Solution {
    // First we find the min value and the max value of bloomDay [bmin, bmax]
    // 
    // Next we write a function that tells us whether some given value of bloomDay (say bval) can satisfy clustering
    // What that means is, if we remove all flowers with bloomDay > bval, 
    // Do the remaining "clusterings" allow us to fulfill the m x k bouquets accounting for adjacency requirements?
    // This is a true/false answer.
    // It sounds like this function is a O(n) function, since we will walk down bloomDay.
    // 
    // Now given the above function, we can perform a binary search on the answer space [bmin, bmax] to find the
    // optimal value of bval
    //
    // Let's assume the bloomDay values range across b
    // The total runtime is O(n log b)

    // canSatisfyClustering checks if we have at least m x k adjacent clusters of flowers,
    // such that each of them have bloomDay <= bval
    bool canSatisfyClustering(const vector<int>& bloomDay, int m, int k, long long bval) {
        int satisfied = 0;
        int bcount=0;
        for (int i=0; i<bloomDay.size(); i++) {
            if (bloomDay[i] > bval) {
                bcount=0;
                continue;
            }
            bcount++;
            if (bcount == k) {
                satisfied++;
                bcount=0;
            }
            if (satisfied >= m) return true;  // early exit
        }
        return satisfied >= m;
    }

    // Binary search for the answer between bmin and bmax.
    int binSearch(const vector<int>& bloomDay, int m, int k, long long bmin, long long bmax) {
        if (bmin == bmax) return bmin;
        if (bmin + 1 == bmax) {
            if (canSatisfyClustering(bloomDay, m, k, bmin)) return bmin;
            return bmax;
        }
        long long bmid = (bmin+bmax)/2;
        if (canSatisfyClustering(bloomDay, m, k, bmid)) {
            return binSearch(bloomDay, m, k, bmin, bmid);
        }
        return binSearch(bloomDay, m, k, bmid+1, bmax);
    }
public:
    int minDays(vector<int>& bloomDay, int m, int k) {
        if ((long long)m*k > bloomDay.size()) return -1;  // not possible.
        int min_val = std::numeric_limits<int>::max();
        int max_val = std::numeric_limits<int>::min();
        for (int b : bloomDay) {
            min_val = std::min(min_val, b);
            max_val = std::max(max_val, b);
        }        
        return binSearch(bloomDay, m, k, min_val, max_val);
    }
};
