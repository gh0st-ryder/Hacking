
struct HeapInfo {
    int index;
    int value;  // dp[index] - psum_ports[index]
};

std::function<bool(const HeapInfo&, const HeapInfo&)> heapcomp = [](const HeapInfo& lhs, const HeapInfo& rhs) -> bool {
    if (lhs.value != rhs.value) {return lhs.value > rhs.value;}  // lesser value on top
    return lhs.index < rhs.index;  // greater index on top for earlier lazy deletion
};

using MinHeap = priority_queue<HeapInfo, vector<HeapInfo>, decltype(heapcomp)>;

class Solution {
    //
    // The current solution is O(n^2) and passes 35/39 test cases.
    // Gets a TLE for the rest.
    // need to optimize the inner loop away
    //
    vector<int> ports;
    vector<long long> weights;
    vector<int> dp;

    // prefix sums for usage in computation later
    vector<long long> psum_weights, psum_ports;

    // MinHeap for calculating the inner work in O(log n) time.
    MinHeap mh;

    // To optimize, we use the minheap above.
    // We re-write the dp equation so that the port costs can be computed via the prefix sums of the ports.
    //
    // for a current trip [i, j-1] the port cost is:
    // pcost = psum_ports[j-1] - psum_ports[i]
    //
    // So the cost to go from i to j is:
    // cost = 2 + (psum_ports[j-1] - psum_ports[i])
    // To this we add the cost of the rest of the trip from j
    // total_cost = dp[j] + 2 + (psum_ports[j-1] - psum_ports[i])
    //
    // For the dp value, we minimize across all such js
    // To do this we can write
    // dp[i] = min-across-j {dp[j] + 2 + psum_ports[j-1] - psum_ports[i]}
    // dp[i] = 2 - psum_ports[i] + min-across-j {dp[j] - psum_ports[j-1]}
    //
    // The quantities in {} we throw in the minheap
    // 
    // When we pop from the min-heap we do lazy deletion:
    // If the index violates maxBoxes constraint or the total weight of the boxes upto that index violates maxWeight constraint
    // Then we just pop and use the next element, until the violations disappear.

    void compute_psums() {
        long long w=0, p=0;
        for (int i=0; i<weights.size(); i++) {
            w += weights[i];
            p += (i > 0 && ports[i] != ports[i-1] ? 1 : 0);
            psum_weights.push_back(w);
            psum_ports.push_back(p);
        }
    }

    bool violates_weight(int i, int j, int maxWeight) {
        // Current trip is from box `i` to box `j-1`
        long long w = ((long long)psum_weights[j-1] - (i > 0 ? psum_weights[i-1]: 0));
        return w  > maxWeight;
    }
    bool violates_boxes(int i, int j, int maxBoxes) {
        // Current trip takes (j-1) - i + 1 = j - i boxes
        return (j - i > maxBoxes);
    }

    HeapInfo compute_heap_elem(int index) {
        int offset = (index == 0 )? 0 : psum_ports[index-1];
        int value = dp[index] + offset;
        return {index, value};
    }

public:
    // O(n log n)
    int boxDelivering(vector<vector<int>>& boxes, int portsCount, int maxBoxes, int maxWeight) {
        const int N = boxes.size();                
        mh = MinHeap(heapcomp);

        for (int i=0; i<boxes.size(); i++) {
            ports.push_back(boxes[i][0]);
            weights.push_back(boxes[i][1]);
        }
        dp = vector<int>(N+1, 0);
        compute_psums();

        // Base case:        
        dp[N] = 0;        
        mh.push(compute_heap_elem(N));

        // DP cases:
        for (int i=N-1; i>=0; i--) {
            HeapInfo chosen;
            while (!mh.empty()) {
                chosen = mh.top();
                if (violates_boxes(i, chosen.index, maxBoxes) || violates_weight(i, chosen.index, maxWeight)) {
                    mh.pop();
                    continue;
                }
                break;
            }       
            int port_val = (i == 0 ? 0 : psum_ports[i]);   
            dp[i] = chosen.value - port_val + 2;
            mh.push(compute_heap_elem(i));
        }
        return dp[0];
    }
};
