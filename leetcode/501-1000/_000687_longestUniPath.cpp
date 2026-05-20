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
    // Cont is returned by traverse.
    struct Cont {
        int longest_path=0;  // the longest path in this subtree
        int longest_from_subroot=0;  // the longest path STARTING from this subroot
    };

    // traverse travels down the tree (both left and right)
    // traverse returns Cont, which contains two values as defined above    
    Cont traverse(TreeNode* node) {
        if (node == nullptr) return {0, 0};

        Cont left, right;
        if (node->left) {
            left = traverse(node->left);
        }
        if (node->right) {
            right = traverse(node->right);
        }
        ;
        int v_path = 0;
        int lp_path=1, rp_path=1;  // the paths with me and potentially my children (if same val).
        if (node->left && node->val == node->left->val && node->right && node->val == node->right->val) {
            v_path = 1 + left.longest_from_subroot + right.longest_from_subroot;
        }
        if (node->left && node->val == node->left->val) {
            lp_path += left.longest_from_subroot;
        }
        if (node->right && node->val == node->right->val) {
            rp_path += right.longest_from_subroot;
        }

        Cont retval;
        int my_best = std::max(std::max(std::max(left.longest_path, right.longest_path),
                                        std::max(lp_path, rp_path)), 
                               v_path);
        int my_longest_from_me = std::max(lp_path, rp_path);
        retval.longest_path = my_best;
        retval.longest_from_subroot = my_longest_from_me;
        return retval;
    }
public:
    int longestUnivaluePath(TreeNode* root) {
        if (root == nullptr) return 0;
        Cont c = traverse(root);
        return c.longest_path - 1;  // only edges, not nodes
    }
};
