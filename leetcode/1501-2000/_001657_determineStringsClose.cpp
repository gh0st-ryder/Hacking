class Solution {
    // The two words must use the same letters
    // They must have the same frequency of groups (not necessarily the same letters)
    //
    // Assuming this happens, we can swap positions and groups as needed to make the words the same... perhaps?    

    void populate(unordered_map<char, int>& m, vector<int>& f, string w) {
        for (char ch : w) {
            m[ch] += 1;
        }
        for (const auto& kv : m) {
            f.push_back(kv.second);
        }
        std::sort(f.begin(), f.end(), std::greater<int>());
    }
public:
    bool closeStrings(string word1, string word2) {
        if (word1.size() != word2.size()) return false;

        unordered_map<char, int> m1, m2;
        vector<int> freq1, freq2;
        populate(m1, freq1, word1);
        populate(m2, freq2, word2);
        if (m1.size() != m2.size()) {
            return false;  // different number of unique letters
        }
        for (const auto& kv : m1) {
            if (m2.find(kv.first) == m2.end()) return false;   // letters are different
        }
        if (freq1.size() != freq2.size()) return false;  // A bit redundant, but OK.
        for (int i=0; i<freq1.size(); i++) {
            if (freq1[i] != freq2[i]) return false;
        }
        return true;
    }
};
