class Solution {
    // We will solve this problem using a union-find data structure.
    vector<int> leaders;
    vector<int> sizes;

    // Key: word; value: index used for that word
    unordered_map<string, int> windex;

    void uf_init(const vector<vector<string>>& similarPairs) {
        for  (int i=0; i<similarPairs.size(); i++) {
            if (windex.find(similarPairs[i][0]) == windex.end()) {            
                windex[similarPairs[i][0]] = leaders.size();
                leaders.push_back(leaders.size());  // be my own leader
                sizes.push_back(1);                 // my group has 1 size
            }
            if (windex.find(similarPairs[i][1]) == windex.end()) {            
                windex[similarPairs[i][1]] = leaders.size();
                leaders.push_back(leaders.size());  // be my own leader
                sizes.push_back(1);                 // my group has 1 size
            }
        }
    }

    void uf_union(const string& word1, const string& word2) {
        int leader1 = uf_find(word1);
        int leader2 = uf_find(word2);
        if (leader1 == leader2) return;
        if (sizes[leader1] < sizes[leader2]) {
            // Change leader1 from being it's own leader to following leader2.
            leaders[leader1] = leader2;
            sizes[leader2] += sizes[leader1];  // leader2's group just grew
        } else {
            // Change leader2 from being it's own leader to following leader1.
            leaders[leader2] = leader1;
            sizes[leader1] += sizes[leader2];  // leader1's group just grew
        }
    }

    // returns the leader for this word
    int uf_find(const string& word) {
        int index = windex[word];
        while (index != leaders[index]) {
            index = leaders[index];
        }
        return index;  // return the leader for this group of words that word belongs to
    }
public:
    bool areSentencesSimilarTwo(vector<string>& sentence1, vector<string>& sentence2, vector<vector<string>>& similarPairs) {
        if (sentence1.size() != sentence2.size()) return false;
        uf_init(similarPairs);

        // Link associations between similar word groups.
        for (int i=0; i<similarPairs.size(); i++) {
            uf_union(similarPairs[i][0], similarPairs[i][1]);
        }

        // Check if sentence positional words are in the same groups.
        for (int i=0; i<sentence1.size(); i++) {
            if (sentence1[i] == sentence2[i]) continue;  // words match
            if (windex.find(sentence1[i]) == windex.end()) return false;  // unknown word not in similarPairs
            if (windex.find(sentence2[i]) == windex.end()) return false;  // unknown word not in similarPairs
            int leader1 = uf_find(sentence1[i]);
            int leader2 = uf_find(sentence2[i]);
            if (leader1 != leader2) return false;
        }
        return true;
    }
};
