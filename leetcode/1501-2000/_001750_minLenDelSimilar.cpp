class Solution {
    // Precondition: s[left] == s[right], left < right
    void iterate(const string& s, int& left, int& right) {
        char ch=s[left];
        // advance left
        while (left < right && s[left] == ch) {
            left++;
        }
        // advance right
        while (left < right && s[right] == ch) {
            right--;
        }
    }
public:
    // O(n) time, O(1) space.
    int minimumLength(string s) {
        if (s.empty()) return 0;

        // left and right mark the next chars to examine
        int left=0, right=s.size()-1;
        char last = ' ';
        while (s[left] == s[right] && left < right) {
            last = s[left];
            iterate(s, left, right);
        }
        if (left == right) {
            if (last == s[left]) return 0;
            return 1;
        }
        return (right-left+1);
    }
};
