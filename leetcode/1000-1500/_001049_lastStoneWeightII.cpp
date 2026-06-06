class Solution {

    // Key: Index i; Value: the possible outcomes if stones up to this index
    // In the end we only consider non-zero possibilities.
    unordered_map<int, unordered_set<int>> possibilities;
public:
    int lastStoneWeightII(vector<int>& stones) {
        // Base case.
        possibilities[0] = {stones[0], -1*stones[0]};

        // DP case.
        for (int i=1; i<stones.size(); i++) {
            for (int prev : possibilities[i-1]) {
                possibilities[i].insert(prev+stones[i]);
                possibilities[i].insert(prev-stones[i]);
            }
        }

        int best = std::numeric_limits<int>::max();
        for (int possible : possibilities[stones.size()-1]) {
            if (possible < 0) continue;
            best = std::min(possible, best);
        }
        return best;
    }
};
