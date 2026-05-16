class Solution {
    struct Record {
        int num;
        string word;
    };
    unordered_map<char, int> letter_counts;
    vector<Record> words = {
        {0, "zero"},
        {2, "two"},
        {6, "six"},
        {8, "eight"},
        {4, "four"},        
        {7, "seven"},
        {3, "three"},
        {5, "five"},           
        {9, "nine"},
        {1, "one"}, 
    };

    bool matches(string word) {
        for (char ch : word) {
            if (letter_counts[ch] == 0) return false;
        }
        return true;
    }

    void eliminate(string word) {
        for (char ch : word) {
            letter_counts[ch]--;
        }
    }
public:
    string originalDigits(string s) {
        for (char ch : s) {
            letter_counts[ch]++;
        }

        string number;
        for (const auto& rec : words) {
            while (matches(rec.word)) {
                eliminate(rec.word);
                number += to_string(rec.num);
            }
        }
        std::sort(number.begin(), number.end());
        return number;
    }
};
