class Solution {
    // Key: digit as a char; Value: count of how many of those in the num
    unordered_map<char, int> counts;

    // Puts the freqs of the chars into counts.
    // Returns the smallest digit that isn't 0 in num.
    char getFreqs(const string& num) {
        char snz = '9';
        for (char ch : num) {
            counts[ch] += 1;
            if (ch < snz && ch != '0') {snz = ch;}
        }
        return snz;
    }
    long long negSmallestNumber(long long num) {
        string snum = std::to_string(num);
        getFreqs(snum);
        string ans = "-";
        for (char ch='9'; ch >= '0'; ch--) {
            string me = string(counts[ch], ch);
            ans += me;
        }
        return std::stoll(ans);
    }
    long long posSmallestNumber(long long num) {
        string snum = std::to_string(num);
        char snz = getFreqs(snum);
        string ans(1, snz);
        counts[snz] -= 1;
        for (char ch='0'; ch <= '9'; ch++) {
            string me = string(counts[ch], ch);
            ans += me;
        }
        return std::stoll(ans);        
    }
public:
    long long smallestNumber(long long num) {
        if (num == 0) return 0;
        if (num < 0) return negSmallestNumber(-1 * num);
        return posSmallestNumber(num);
    }
};
