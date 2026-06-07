class Solution {
    // 
    // First sort the prices.
    // Next consider maxdiff in the range [0, max_price] => this is the solution space, our answer lies in this range.
    // We know that if we set maxdiff to 0, we can find k distinct candies.
    // We also know that if we set maxdiff to max_price, we cannot find k distinct candies
    // We will therefore binary search across this solution space to find the answer.
    //
    // We will use a helper function for the binary search.
    // Given a diff_val in the above range, it will go through the sorted price container
    // And it will answer whether we can pick k values from this container such that 
    // the max abs diff between the values is >= diff_val
    //
    // To go through the sorted price container itself we could also conduct a secondary binary search
    // At first I plan to implement just the naive linear scan
    // But if that leads to TLE, we can switch to this secondary binary search.
    //

    // Consider n = cardinality of price
    //          m = the max price possible

    // This function is O(n);
    bool canFulfillK(const vector<int>& price, int k, int diff) {
        if (price.size() == 0 || price.size() == 1) return false;
        int picked=1, index=1, last = price[0];
        while (index < price.size() && picked < k) {
            while (index < price.size() && std::abs(last - price[index]) < diff) {
                index++;
            }
            if (index < price.size()) {
                picked++;
                last = price[index];
            }
        }
        return picked == k;
    }

    // This algorithm is O(n log m)
    int binSearchSolution(const vector<int>& price, int k, int minv, int maxv) {
        if (minv == maxv) return minv;
        if (minv+1 == maxv) {
            if (canFulfillK(price, k, maxv)) return maxv;
            return minv;
        }
        int midv = (maxv + minv)/2;
        if (canFulfillK(price, k, midv)) {
            return binSearchSolution(price, k, midv, maxv);
        }
        return binSearchSolution(price, k, minv, midv);
    }
public:
    // O(n log n + n log m).
    int maximumTastiness(vector<int>& price, int k) {
        std::sort(price.begin(), price.end());
        int max_val = std::numeric_limits<int>::min();
        for (int p : price) {max_val = std::max(max_val, p);}

        int ans = binSearchSolution(price, k, 0, max_val);
        return ans;
    }
};
