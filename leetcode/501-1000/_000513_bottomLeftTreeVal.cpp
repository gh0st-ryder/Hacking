/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
    struct Cont {
        TreeNode *node;
        int level=0;
    };
public:
    int findBottomLeftValue(TreeNode* root) {
        if (root == nullptr) return -1;  // should never happen
        std::deque<Cont> myq;
        myq.push_back({root, 0});
        int last=0;  // the level currently being processed
        int leftmost=root->val; // find the leftmost
        while (!myq.empty()) {
            Cont elem = myq.front(); myq.pop_front();
            if (elem.level != last) {
                leftmost = elem.node->val;
                last = elem.level;
            }
            if (elem.node->left) {myq.push_back({elem.node->left, elem.level+1});}
            if (elem.node->right) {myq.push_back({elem.node->right, elem.level+1});}
        }
        return leftmost;
    }
};
