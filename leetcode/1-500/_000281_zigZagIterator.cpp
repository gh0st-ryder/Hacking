class ZigzagIterator {
    vector<int> v1_, v2_;
    int v1_next=-1, v2_next=-1;  // will set to -1 when all consumed
    int next_turn = 0; // 0 = done; else 1/2 is v1/v2 turn
public:
    ZigzagIterator(vector<int>& v1, vector<int>& v2) {
        v1_ = v1;
        v2_ = v2;
        if (v1_.size() > 0) v1_next = 0;
        if (v2_.size() > 0) v2_next = 0;
        if (v1_.size() > 0) {
            next_turn = 1;
        } else if (v2_.size() > 0) {
            next_turn = 2;
        } 
    }

    int next() {
        int ret=0;
        if (!hasNext()) return -1;  // invalid to call this method
        if (next_turn == 1) {  // v1's turn, invariant is that there is at least one element to consume
            ret = v1_[v1_next++];
            if (v1_next == v1_.size()) v1_next = -1; // all consumed
            if (v2_next == -1) {
                next_turn = (v1_next == -1 ? 0 : 1);   // keep consuming from 1
            } else {
                next_turn = 2;
            }
        } else {  // v2's turn, invariant is that there is at least one element to consume
            ret = v2_[v2_next++];
            if (v2_next == v2_.size()) v2_next = -1; // all consumed
            if (v1_next == -1) {
                next_turn = (v2_next == -1 ? 0 : 2);  // keep consuming from 2
            } else {
                next_turn = 1;            
            }
        }
        return ret;
    }

    bool hasNext() {
        return next_turn != 0;
    }
};

/**
 * Your ZigzagIterator object will be instantiated and called as such:
 * ZigzagIterator i(v1, v2);
 * while (i.hasNext()) cout << i.next();
 */
