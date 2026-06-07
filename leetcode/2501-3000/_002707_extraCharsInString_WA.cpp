//
// Fuck me! This question is currently giving a WA. 
// 1994/2028 cases passed.
//
// The trie is itself the correct approach. It is correctly implemented and working well.
// 
// However, using a greedy strategy of matching the biggest word doesn't work in some edge cases.
// Specifically, if we match "leet" as well as "leetcode", we need to use DP to try both cases.
// This is because downstream we may have a word "codebreakerfoobar" 
// And if our blurb was "leetcodebreakerfoobar" then matching just the greedy strategy fails.
//

struct TrieNode {    
    bool is_end = false;
    unordered_map<char, TrieNode*> children;

    TrieNode() {init();}
    void init() {
        for (char ch='a'; ch<='z'; ch++) {
            children[ch] = nullptr;
        }
    }
    ~TrieNode() {
        for (const auto& kv : children) {
            if (kv.second != nullptr) delete kv.second;
        }
    }
    void addWord(const string& word, int index=-1) {
        if (index >= (int)(word.size())) return;        
        if (index == (int)word.size()-1) {
            // printf("Marked is_end at level %d\n", index);
            is_end = true;
            return;
        }
        index++;   
        TrieNode *next = children[word[index]];
        if (next==nullptr) {
            next = children[word[index]] = new TrieNode();
        }
        // printf("Added letter %c at level %d\n", word[index], index);
        next->addWord(word, index);
    }

    // matchWord walks the trie, trying to match blurb against the biggest words it currently has.
    // cur_index is used to track the index in blurb for recursion.
    // index is modified to the offset in blurb 1 beyond the last letter matched (with is_end=true at that letter)
    // returns true if something matched, else false.
    bool matchWord(const string& blurb, int cur_index, int& index) {    
        // printf("Trying letter %c at blurb index %d\n", blurb[cur_index], cur_index);    
        if (cur_index == blurb.size() || children[blurb[cur_index]] == nullptr) {
            return false;
        }
        if (children[blurb[cur_index]]->is_end) {            
            index = cur_index+1;
            // printf("Set found word index to %d\n", index);
        }
        bool rec = children[blurb[cur_index]]->matchWord(blurb, cur_index+1, index);
        return (rec || children[blurb[cur_index]]->is_end);
    }
};

class Solution {
    TrieNode* root = nullptr;

public:
    int minExtraChar(string s, vector<string>& dictionary) {
        root = new TrieNode();

        // Add words to the retrieval tree.
        for (const string& word : dictionary) {
            root->addWord(word);
        }

        // Find matches and manipulate the data.
        int skipped=0, curr=0;
        while (curr < s.size()) {
            int index=-1;
            bool is_match = root->matchWord(s, curr, index);
            if (is_match) {
                // printf("Found a match at index %d\n", index);
                curr = index;
            } else {
                skipped++;
                curr++;
            }
        }
        return skipped;
    }
};
