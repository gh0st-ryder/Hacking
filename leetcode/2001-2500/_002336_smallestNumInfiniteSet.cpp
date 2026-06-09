class SmallestInfiniteSet {
    int next_infinite=1;
    // minheap
    std::priority_queue<int, vector<int>, std::greater<int>> heap;
    unordered_set<int> added_back;
public:
    SmallestInfiniteSet() {
        
    }
    
    int popSmallest() {
        if (!heap.empty()) {
            int num = heap.top();
            heap.pop();
            added_back.erase(num);
            return num;
        }
        int ans = next_infinite;
        next_infinite += 1;
        return ans;
    }
    
    void addBack(int num) {
        if (num >= next_infinite || added_back.find(num) != added_back.end()) return;
        heap.push(num);
        added_back.insert(num);
    }
};

/**
 * Your SmallestInfiniteSet object will be instantiated and called as such:
 * SmallestInfiniteSet* obj = new SmallestInfiniteSet();
 * int param_1 = obj->popSmallest();
 * obj->addBack(num);
 */
