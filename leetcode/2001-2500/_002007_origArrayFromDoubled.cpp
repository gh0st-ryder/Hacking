class Solution {
public:
    vector<int> findOriginalArray(vector<int>& changed) {
        if (changed.size()%2) return {};  // need even number of elems.
        vector<int> ans;
        map<int, int> freqs;
        for (int n : changed) {freqs[n] += 1;}

        // Special case treatment for zero
        if (freqs.find(0) != freqs.end()) {
            if (freqs[0]%2) return {};  // odd number of zeros is incorrect
            for (int i=0; i<freqs[0]/2; i++) {
                ans.push_back(0);
            }
            freqs[0] = 0;
        }

        for (const auto& kv : freqs) {
            // Safe to change values only while iterating.
            if (kv.second == 0) continue;  // already accounted for and removed
            if (freqs.find(kv.first*2) == freqs.end()) return {};  // no doubled values present
            int double_freq = freqs[kv.first*2];
            if (double_freq < kv.second) return {};  // not enough matches
            for (int i=0; i<kv.second; i++) {
                ans.push_back(kv.first);
            }
            freqs[kv.first*2] -= kv.second;  // remove enough from kv.first*2 to get rid of the matches.
        }
        return ans;
    }
};
