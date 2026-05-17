class Solution {
    string magical_;

    void flip(char& ch) {
        if (ch == '1') {
            ch = '2';
        } else {
            ch = '1';
        }
    }

    void generateMagical() {
        magical_ = "122112";
        magical_.reserve(100005);
        int left = 4;  // start from here; left is index saying the number of new letters we are generating
        char ch='1';  // the next char to generate      
        while (magical_.size() <= 100000) {
            if (magical_[left] == '1') {
                // Generate a single val.
                magical_.push_back(ch);                
            } else {
                // Generate two vals.
                magical_.push_back(ch);
                magical_.push_back(ch);
            }
            flip(ch);
            left++;
        }
    }

public:
    int magicalString(int n) {
        if (magical_.empty()) {
            generateMagical();
        }
        int count=0;
        for (int i=0; i<n; i++) {
            if (magical_[i] == '1') count++;
        }
        return count;
    }
};
