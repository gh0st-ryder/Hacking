struct Info {
    int val=0;
    bool from_left=true;  // whether to get the next from the left or right
};

std::function<bool(const Info&, const Info&)> comp = [](const Info& lhs, const Info& rhs) -> bool {
    if (lhs.val != rhs.val) return lhs.val > rhs.val;  // minheap
    int lhs_int = (lhs.from_left ? 0 : 1);
    int rhs_int = (rhs.from_left ? 0 : 1);
    return lhs_int > rhs_int;  // choose the smallest index
};

using MinHeap = std::priority_queue<Info, vector<Info>, decltype(comp)>;

class Solution {
public:
    long long totalCost(vector<int>& costs, int k, int candidates) {
        MinHeap minheap(comp);
        int left=0, right=costs.size()-1;
        int used=0;
        if (candidates * 2 >= costs.size()) {
            // Everything is going into the heap.
            for (int c : costs) {
                minheap.push({c, true});
            }
            used = costs.size();
        } else {
            used = 2*candidates;
            for (; left < candidates; left++) {minheap.push({costs[left], true});}            
            for (int i=0; i<candidates; i++, right--) {minheap.push({costs[right], false});}
        }
        long long total=0;
        for (int i=0; i<k; i++) {
            Info info = minheap.top();
            minheap.pop();
            total += info.val;
            if (used < costs.size()) {
                used++;
                if (info.from_left) {
                    minheap.push({costs[left], true});
                    left++;
                } else {
                    minheap.push({costs[right], false});
                    right--;
                }
            }
        }
        return total;
    }
};
