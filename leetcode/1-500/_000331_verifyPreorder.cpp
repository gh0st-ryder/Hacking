class Solution {
    string preorder_;
    int ptr_=0;  // the next index to deserialize

    // Starts at the current ptr_ value.
    // Attempts to gobble up the contents until it "finishes" its own tree
    // At the end, the ptr_ is left at the next index beyond the content it gobbled up.
    // That's assuming the content it gobbled up was a valid subtree.
    // returns true if successful in gobbling a tree, false otherwise
    bool deserialize() {
        if (ptr_ >= preorder_.size()) return false;  // nothing to gobble up.

        int index = preorder_.find_first_of(",", ptr_);
        
        if ((index == ptr_+1 || index == string::npos) && preorder_[ptr_] == '#') {
            // Matched a null tree.
            ptr_ += 1 + (index == string::npos ? 0 : 1);  // Match '#' and comma
            return true;
        }

        // gobble up the subroot value.
        ptr_ = (index == string::npos ? preorder_.size() : index+1);  // go to just beyond the comma

        if (!deserialize()) {
            return false;  // didn't match left
        }
        if (!deserialize()) {
            return false;  // didn't match right
        }
        // Matched root, left, right, and consumed all my portions.
        return true;
    }

public:
    bool isValidSerialization(string preorder) {
        if (preorder.size() == 0) return false;
        preorder_ = preorder;
        bool matches = deserialize();
        // the whole string is one big binary tree, and I also consumed everything
        return matches && (ptr_ == preorder_.size());
    }
};
