class Solution {
    unordered_map<int, int> prefix_sum, suffix_sum;

public:
    int bestClosingTime(string customers) {
        int sum=0;
        int penalty=1;
        // prefix_sum denotes the accrued penalty such that
        // The shop closed at index i, and we accrued the penalty for all N's prior to index i.
        for (int i=0; i<customers.size(); i++) {
            prefix_sum[i] = sum;
            if (customers[i] == 'N') {
                sum += penalty;                
            }
        }

        // suffix_sum denotes the accrued penalty such that
        // The shop closed at index i, and we accrued the penalty for all Y's after index i.
        sum=0;
        penalty=1;
        for (int i=customers.size()-1; i>=0; i--) {
            if (customers[i] == 'Y') {
                sum += penalty;                
            }
            suffix_sum[i] = sum;
        }

        int best = std::numeric_limits<int>::max();
        int best_index = -1;
        for (int i=0; i<customers.size(); i++) {
            int curr_penalty = prefix_sum[i] + suffix_sum[i];
            if (curr_penalty < best) {                
                best = curr_penalty;
                best_index = i;
            }
        }

        // The above assumes that the shop at least closes at the last hour, since i is the index that the shop was closed.
        // So we also need to check specially if the shop shouldn't be closed at these hours at all!
        // Let's construct a starting case where we don't close at all.        
        sum = 0;
        penalty=1;
        for (int i=0; i<customers.size(); i++) {
            if (customers[i] == 'N') {
                sum += penalty;
            }
        }
        if (sum < best) {
            best_index = customers.size();
        }
        return best_index;
    }
};
