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
    TreeNode* ans = nullptr;

    // Returns the number of nodes from match that are matched by the sub-tree at node.
    int dfs(const unordered_set<TreeNode*>& match, TreeNode* node) {
        if (node == nullptr) return 0;
        int lhs = dfs(match, node->left);
        int rhs = dfs(match, node->right);
        int total = lhs + rhs;
        if (match.find(node) != match.end()) {
            total += 1;
        }
        if (total == match.size() && ans == nullptr) {
            ans = node;
        }
        return total;
    }
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, vector<TreeNode*> &nodes) {
        unordered_set<TreeNode*> match;
        for (TreeNode* node : nodes) {match.insert(node);}
        dfs(match, root);
        return ans;
    }
};
