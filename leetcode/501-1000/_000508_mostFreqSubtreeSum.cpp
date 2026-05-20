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
    unordered_map<int, vector<int>> subtree_sums;

    // returns subtree sum at root
    int traverse(TreeNode* root) {
        if (root == nullptr) return 0;
        int left = traverse(root->left);
        int right = traverse(root->right);
        int my_sum = root->val + left + right;
        subtree_sums[my_sum].push_back(root->val);
        return my_sum;
    }
public:
    vector<int> findFrequentTreeSum(TreeNode* root) {
        int freq=0;
        vector<int> result;
        traverse(root);
        for (const auto& kv: subtree_sums) {
            if (kv.second.size() > freq) {
                freq = kv.second.size();
                result = vector<int>(1, kv.first);
            } else if (kv.second.size() == freq) {
                result.push_back(kv.first);
            }
        }
        return result;
    }
};
