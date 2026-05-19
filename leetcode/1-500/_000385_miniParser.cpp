/**
 * // This is the interface that allows for creating nested lists.
 * // You should not implement it, or speculate about its implementation
 * class NestedInteger {
 *   public:
 *     // Constructor initializes an empty nested list.
 *     NestedInteger();
 *
 *     // Constructor initializes a single integer.
 *     NestedInteger(int value);
 *
 *     // Return true if this NestedInteger holds a single integer, rather than a nested list.
 *     bool isInteger() const;
 *
 *     // Return the single integer that this NestedInteger holds, if it holds a single integer
 *     // The result is undefined if this NestedInteger holds a nested list
 *     int getInteger() const;
 *
 *     // Set this NestedInteger to hold a single integer.
 *     void setInteger(int value);
 *
 *     // Set this NestedInteger to hold a nested list and adds a nested integer to it.
 *     void add(const NestedInteger &ni);
 *
 *     // Return the nested list that this NestedInteger holds, if it holds a nested list
 *     // The result is undefined if this NestedInteger holds a single integer
 *     const vector<NestedInteger> &getList() const;
 * };
 */
class Solution {
    string repr_;
    int ptr_=0;  // points to the next char to be consumed.

    // parses a single Int
    // does NOT adjust for [, ], or commas
    int parseInt() {
        int index = repr_.find_first_of(",]", ptr_);
        int len_int = index - ptr_;
        string int_str = repr_.substr(ptr_, len_int);        
        int int_val = std::stoi(int_str); 
        ptr_ = (index == string::npos ? repr_.size() : index);
        return int_val;       
    }

    // Parses a Nested Integer
    // Can be a single NestedInteger, or a comma separated list of NestedInteger
    // Where a NestedInteger can also be just an integer
    // Adjusts for "[],"
    NestedInteger parseList() {
        NestedInteger ni;      
        ptr_++;  // gobble up the '['  

        while (1) {
            if (repr_[ptr_] == ']') {
                // done here
                ptr_++;  // gobble up the ']'
                return ni;
            }            
            // handles both a raw int, or a list
            NestedInteger inside = parse();
            ni.add(inside); 

            // gobble up a comma if it exists
            if (ptr_ != repr_.size() && repr_[ptr_] == ',') {
                ptr_++;  // gobble up the ','
            }                       
        }

        return ni;
    }

    NestedInteger parse() {        
        if (repr_[ptr_] == '[') {            
            NestedInteger ni = parseList();            
            return ni;
        } else {
            int ni = parseInt();
            return NestedInteger(ni);
        }
    }
public:
    NestedInteger deserialize(string s) {       
        repr_ = s;
        return parse();
    }
};
