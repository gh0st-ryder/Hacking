class Solution {
    // Key: letter; Value: freq of that letter in the word
    unordered_map<char, int> freq1, freq2;

    bool bruteForce(string word1, string word2) {        
        for (char ch : word1) { freq1[ch] += 1; }
        for (char ch : word2) { freq2[ch] += 1; }

        // Try to swap ch1 from w1 and ch2 from w2 and see if the words are the same
        for (char ch1='a'; ch1<='z'; ch1++) {
            if (freq1.find(ch1) == freq1.end()) continue;
            for (char ch2='a'; ch2<='z'; ch2++) {
                if (freq2.find(ch2) == freq2.end()) continue;

                // Make the swap
                unordered_map<char, int> f1 = freq1, f2=freq2;
                f1[ch1] -= 1; f1[ch2] += 1;
                if (f1[ch1] == 0) f1.erase(ch1);
                f2[ch2] -= 1; f2[ch1] += 1;
                if (f2[ch2] == 0) f2.erase(ch2);

                if (f1.size() == f2.size()) return true;
            }
        }
        return false;
    }
public:
    bool isItPossible(string word1, string word2) {
        return bruteForce(word1, word2);
    }
};
