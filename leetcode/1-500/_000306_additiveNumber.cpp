class Solution {
    // The largest long long that we can have
    string largest = "9223372036854775807";
    bool doesntFit(string tnum, int sz) {        
        if (sz > 19) return true;
        if (tnum.length() < 19) {
            tnum.insert(0, 19 - tnum.length(), '0');
        }
        return tnum > largest;
    }
    // isAdditive returns true if the string is additive
    // index is the current index to start matching the next number from
    // pnum and ppnum are the prev and prev_prev numbers (-1 if invalid).
    // count is numbers parsed so far
    bool isAdditive(const string& num, int index, long long pnum, long long ppnum, int count) {
        if (index >= num.size()) return count >= 3;        

        // Get all possibilities for this index.
        int max_size = num.size() - index;
        for (int sz=1; sz <= max_size; sz++) {
            string tnum = num.substr(index, sz);  // can't be string::npos

            if (tnum[0] == '0' && sz > 1) continue;  // skip numbers with a leading 0
            if (doesntFit(tnum, sz)) continue;  // skip numbers that don't convert using stoll

            long long this_num = std::stoll(tnum);            
            if (pnum != -1 && ppnum != -1) {                
                if (this_num != pnum + ppnum) continue;  // this is not a valid break
            }
            if (isAdditive(num, index+sz, this_num, pnum, count+1)) {                
                return true;
            }
            // if not additive, continue to next break
        }        
        return false;
    }
public:
    bool isAdditiveNumber(string num) {
        if (num.size() <= 2) return false;
        return isAdditive(num, 0, -1, -1, 0);
    }
};
