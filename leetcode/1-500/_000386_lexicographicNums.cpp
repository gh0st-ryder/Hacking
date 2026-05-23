struct TrieNode {    
    char ch = ' ';
    vector<TrieNode*> children;
    TrieNode(char c) : ch(c) {
        for (int i=0; i<10; i++) {
            children.push_back(nullptr);
        }
    }
    ~TrieNode() {
        for (TrieNode* node : children) delete node;
    }
};

class Solution {    
    TrieNode *root = nullptr;
    vector<int> answer;

    void addNum(TrieNode*& node, string num, int level) {
        if (level >= num.size()) return;          
        if (node == nullptr) {node = new TrieNode(num[level]); }                     
        
        if (level + 1 < num.size()) {
            int index = num[level+1] - '0';
            string number = num.substr(0, level+1);            
            addNum(node->children[index], num, level+1);
        }
    }

    void getAnswer(TrieNode* node, string& running_num) {
        if (node == nullptr) return;
        running_num.push_back(node->ch);
        answer.push_back(std::stoi(running_num));
        for (int i=0; i<node->children.size(); i++) {
            getAnswer(node->children[i], running_num);
        }
        running_num.pop_back();
    }
    
public:
    ~Solution() {delete root;}

    vector<int> lexicalOrder(int n) {
        root = new TrieNode(' ');

        // First populate the trie.
        for (int i=1; i<=n; i++) {
            string num = std::to_string(i);
            addNum(root->children[num[0] - '0'], num, 0);
        }

        // Then read it in sequence and get the answer.
        string running_num;
        for (TrieNode * node : root->children) {
            getAnswer(node, running_num);
        }
        return answer;
    }
};
