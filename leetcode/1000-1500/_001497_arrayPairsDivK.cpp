class Solution {
    // Key: ranges from [0 -> k-1]  => i.e. n%k
    // Value: count of how many numbers satisfy this
    unordered_map<int, int> modulo_count;
    
public:
    bool canArrange(vector<int>& arr, int k) {
        for (int n : arr) {
            modulo_count[n%k] += 1;
        }
        // First check the remainder of 0        
        if (modulo_count[0] % 2) return false;  // one unmatched number        
        
        // eat up the smaller number of combinations of [p and (k-p)] and [-p and -(k-p)]
        for (int check=1; check <= k/2; check++) {
            if (check * 2 == k) {  // p and (k-p) are the same.
                // eat up as many check's as I can
                modulo_count[check] %= 2;

                // eat up as many -check's as I can
                modulo_count[-1*check] %= 2;

                // match remaining check's with -check's
                if (modulo_count[check] != modulo_count[-1*check]) return false;
            }  else {
                // eat up as many check and (k-check) as I can
                int minv = std::min(modulo_count[check], modulo_count[k-check]);
                modulo_count[check] -= minv;
                modulo_count[k-check] -= minv;

                // eat up as many -check and -(k-check) as I can
                minv = std::min(modulo_count[-1*check], modulo_count[-1*(k-check)]);
                modulo_count[-1* check] -= minv;
                modulo_count[-1 * (k-check)] -= minv;

                // if check and -check don't match, flag it
                if (modulo_count[check] != modulo_count[-1*check]) return false;

                // if (k-check) and -(k-check) don't match, flag it
                if (modulo_count[k-check] != modulo_count[-1*(k-check)]) return false;
            }
        }
        return true;
    }
};
