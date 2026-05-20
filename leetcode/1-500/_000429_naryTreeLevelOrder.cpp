/*
// Definition for a Node.
class Node {
public:
    int val;
    vector<Node*> children;

    Node() {}

    Node(int _val) {
        val = _val;
    }

    Node(int _val, vector<Node*> _children) {
        val = _val;
        children = _children;
    }
};
*/

class Solution {
    struct Cont {
        Node* node;
        int level=0;
    };
public:
    vector<vector<int>> levelOrder(Node* root) {
        if (root == nullptr) return {};
        vector<vector<int>> result(1, vector<int>());
        deque<Cont> myq;
        myq.push_back({root, 0});

        int last = 0;  // the last level we were looking at
        while (!myq.empty()) {
            Cont elem = myq.front(); myq.pop_front();
            if (elem.level != last) {                
                last = elem.level;
                result.push_back(vector<int>());
            }
            result.back().push_back(elem.node->val);
            for (Node* child : elem.node->children) {
                myq.push_back({child, elem.level+1});
            }
        }
        return result;
    }
};
