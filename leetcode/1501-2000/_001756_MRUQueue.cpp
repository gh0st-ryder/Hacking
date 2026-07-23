class MRUQueue {
    std::list<int> llist;
public:
    MRUQueue(int n) {
        for (int i=1; i<=n; i++) {
            llist.push_back(i);
        }
    }
    
    int fetch(int k) {
        if (k <= 0) return -1;
        if (k > llist.size()) return -1;
        auto it = llist.begin();
        for (int i=1; it != llist.end(); it++, i++) {
            if (i == k) break;
        }        
        int ret = *it;
        llist.erase(it);
        llist.push_back(ret);
        return ret;
    }
};

/**
 * Your MRUQueue object will be instantiated and called as such:
 * MRUQueue* obj = new MRUQueue(n);
 * int param_1 = obj->fetch(k);
 */
