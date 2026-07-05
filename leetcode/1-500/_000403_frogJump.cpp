class Solution {
    unordered_set<int> stone_indices;
    int terminal=0;

    // Key: pair of (index, last jump val to get at this index)
    //      note that here index is the value of stones[i]
    // Value: Whether it solves the problem.
    unordered_map<long long, bool> cache;

    long long makeKey(int index, int jump) {
        return (((long long)index) | (((long long)jump) << 32));
    }

    // Recurrence relation
    // solve(i, j) = if (i == lastStoneIndex) then true
    // solve(i, j) = otherwise => for k in filter(j-1, j, j+1) boolean or across {solve(i+k, k)}
    //                            where filter(k) = k if k > 0 else drop k

    // The function solve tells us that if we land at index "index" with a jump value of "jump"
    // then does it solve the problem (i.e. do we ultimately end up on the last stone position).
    bool solve(int index, int jump) {
        if (index == terminal) return true;
        long long key = makeKey(index, jump);
        if (cache.find(key) != cache.end()) return cache[key];

        bool ans = false;
        // Find the candidates jumps possible and try them.
        vector<int> delta = {jump - 1, jump, jump + 1};
        for (int d : delta) {
            if (d <= 0) continue;  // can only jump forward, not in place or backward
            int target = index + d;
            if (stone_indices.find(target) == stone_indices.end()) continue;  // no stone at that position
            bool rec = solve(target, d);
            if (rec) {
                ans = true;
                break;
            }
        }
        cache[key] = ans;
        return ans;
    }
public:
    bool canCross(vector<int>& stones) {
        // Check if the first jump is even possible
        if (stones[1] != stones[0] + 1) return false;
        
        terminal = stones.back();  // the index of the place that solves the problem.
        for (int ind : stones) {stone_indices.insert(ind);}

        return solve(stones[1], 1);
    }
};
