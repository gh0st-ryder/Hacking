class Solution {
    // Each word is precomputed as follows:
    // It is turned into a size 26 string, in english alphabet order.
    // If that letter is present in the word, it is present at the proper position in precomputed
    // If that letter is absent in the word, we replace it with underscore.
    // e.g. bad => ab_d_________<22 times>
    vector<string> precomputed;

    int hasCommon(const string& pc1, const string& pc2) {
        for (int i=0; i<26; i++) {
            if (pc1[i] == '_' || pc2[i] == '_') continue;
            return true;
        }
        return false;
    }
public:
    // if words.size() == N and words[i].size() == M
    
    // O(M*N log M + N^2) time.
    int maxProduct(vector<string>& words) {
        // Do the precomputations.
        // O(M*N log M) time for the precomputations.
        // The set find takes log M time.
        for (const string& word : words) {
            std::set<char> letters;
            for (char ch : word) {
                letters.insert(ch);
            }
            string precompute;
            for (char ch='a'; ch <= 'z'; ch++) {
                if (letters.find(ch) != letters.end()) {
                    precompute.push_back(ch);
                } else {
                    precompute.push_back('_');
                }
            }
            precomputed.push_back(precompute);
        }

        int max = 0;
        // O(N^2) time.
        for (int i=0; i<words.size(); i++) {
            for (int j=0; j<words.size(); j++) {
                if (!hasCommon(precomputed[i], precomputed[j])) {
                    int prod = words[i].size() * words[j].size();
                    if (prod > max) max = prod;
                }
            }
        }
        return max;
    }
};
