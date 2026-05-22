class Solution {
    vector<string> start = {
        "1", "2", "3", "4", "5", "6", "7", "8", "9"
    };

    // Grows the current strings by 1 digit on the right, by a difference of k
    vector<string> grow(const vector<string>& input, int k) {
        vector<string> ret;
        for (string num : input) {
            int digit = std::stoi(string(1, num.back()));
            // Try to grow it forward
            int fwd = digit + k;
            if (fwd < 10) {
                ret.push_back(num + std::to_string(fwd));
            }
            // Try to grow it backward
            int back = digit - k;
            if (back >= 0 && back != fwd) {
                ret.push_back(num + std::to_string(back));
            }
        }
        return ret;
    }
public:
    vector<int> numsSameConsecDiff(int n, int k) {
        vector<string> grown = start;
        for (int i=2; i<=n; i++) {
            grown = grow(grown, k);
        }
        vector<int> ret;
        for (string str : grown) {
            ret.push_back(std::stoi(str));
        }
        return ret;
    }
};
