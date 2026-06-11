class Solution {
    bool isVowel(char ch) {
        return (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u');
    }
public:
    int maxVowels(string s, int k) {
        if (s.size() < k) return 0;
        int best = std::numeric_limits<int>::min();

        // Prework
        int count=0;
        for (int i=0; i<k; i++) {
            if (isVowel(s[i])) {
                count += 1;
                best = std::max(best, count);
            }
        }

        // Main work
        for (int left=0, right=k; right < s.size(); right++, left++) {
            // first remove the oldest
            if (isVowel(s[left])) {
                count -= 1;
            }
            // check if the new one affects the result
            if (isVowel(s[right])) {
                count += 1;
            }
            best = std::max(best, count);
        }
        return best;
    }
};
