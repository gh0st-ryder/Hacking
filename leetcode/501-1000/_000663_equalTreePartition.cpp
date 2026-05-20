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
    int total=0;
    unordered_map<TreeNode*, int> subtree_sums;

    int computeTotal(TreeNode* node) {
        if (node == nullptr) return 0;
        int my_total = node->val + computeTotal(node->left) + computeTotal(node->right);
        subtree_sums[node] = my_total;
        return my_total;
    }

    bool check(TreeNode *node) {
        if (node == nullptr || (node->left == nullptr && node->right == nullptr)) return false;

        // Does breaking the left link if possible satisfy the check?
        if (node->left) {            
            int without_left = total - subtree_sums[node->left];
            if (without_left == subtree_sums[node->left]) return true;
        }

        // Does breaking the right link if possible satisfy the check?
        if (node->right) {
            int without_right = total - subtree_sums[node->right];            
            if (without_right == subtree_sums[node->right]) return true;
        }

        if (check(node->left)) return true;
        if (check(node->right)) return true;
        return false;
    }
public:
    bool checkEqualTree(TreeNode* root) {
        if (root == nullptr) return false;
        total = computeTotal(root);
        return check(root);
    }
};
