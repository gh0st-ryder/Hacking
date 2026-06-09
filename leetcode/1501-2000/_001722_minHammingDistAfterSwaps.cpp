class Solution {
    // Observation:
    // Swaps can be transitive! 
    // So if you have indices [a, b], [b, c], [c, d], [d, e]
    //   then any values in indices a, b, c, d can be rearranged in any order at those indices!
    //   This grouping a, b, c, d, e is essentially a connected component.
    //
    // What this means is, we find these connected components, perhaps using union find.
    // Then we find the number of 1's and the number of 0's in each group, in both the source and target
    // Then we find how many are mismatched for each group (just need either 1 mismatches or 0 mismatches).
    // 
    // Add the number of mismatches for all groups, and that is the minimum hamming distance.

    // Union find implementation.
    vector<int> leaders;
    vector<int> sizes;

    void uf_init(int total) {
        for (int i=0; i<total; i++) {
            leaders.push_back(i);
            sizes.push_back(1);
        }
    }
    int uf_find(int node) {        
        while (node != leaders[node]) {
            node = leaders[node];
        }
        return node;
    }
    void uf_union(int node1, int node2) {
        int leader1 = uf_find(node1);
        int leader2 = uf_find(node2);
        if (leader1 == leader2) return;
        // printf("l1=%d; l2=%d => s1=%d, s2=%d\n", leader1, leader2, sizes[leader1], sizes[leader2]);
        if (sizes[leader1] >= sizes[leader2]) {
            leaders[leader2] = leader1;
            sizes[leader1] += sizes[leader2];
        } else {
            leaders[leader1] = leader2;
            sizes[leader2] += sizes[leader1];
        }
    }

    // Here is where we have the groups
    // Key: leader; Value: all the nodes in that leader's group
    unordered_map<int, unordered_set<int>> groups;

  public:
    int minimumHammingDistance(vector<int>& source, vector<int>& target, vector<vector<int>>& allowedSwaps) {
        uf_init(source.size());
        for (const vector<int>& swap : allowedSwaps) {
            uf_union(swap[0], swap[1]);
        }        

        for (int i=0; i<source.size(); i++) {
            int leader = uf_find(i);
            groups[leader].insert(i);            
        }

        int mismatches=0;
        for (const auto& kv : groups) {
            // num to freq
            unordered_map<int, int> ss, ts;
            int total=0;
            for (int index : kv.second) {
                total++;
                ss[source[index]] += 1;
                ts[target[index]] += 1;
            }            
            int matches=0;
            for (const auto& kvs : ss) {
                if (ts.find(kvs.first) != ts.end()) {
                    matches += std::min(ts[kvs.first], kvs.second);
                }
            }
            mismatches += (total - matches);
        }
        return mismatches;
    }
};
