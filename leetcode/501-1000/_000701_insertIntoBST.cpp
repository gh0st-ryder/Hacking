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
    // Maintains the invariant that this will not be called with a nullptr node.
    void insert(TreeNode *node, int val) {
        if (val <= node->val) {
            if (node->left == nullptr) {
                TreeNode *child = new TreeNode(val);
                node->left = child;
                return;
            }
            insert(node->left, val);
        } else {
            if (node->right == nullptr) {
                TreeNode *child = new TreeNode(val);
                node->right = child;
                return;
            }
            insert(node->right, val);            
        }
    }
public:
    TreeNode* insertIntoBST(TreeNode* root, int val) {
        if (root == nullptr) {
            TreeNode *node = new TreeNode(val);
            return node;
        }
        insert(root, val);
        return root;        
    }
};
