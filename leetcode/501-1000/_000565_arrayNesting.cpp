class Solution {
    // Key is an index from 0 to n-1
    // Value is the length of the chain that starts at this index 
    // and ends in a dead end or a cycle (duplicate)
    unordered_map<int, int> chain_from;

    int populateChain(const vector<int>& nums, int index) {
        if (chain_from.find(index) != chain_from.end()) {
            return chain_from[index];  // already done
        }
        if (index == nums[index]) {
            // Found a dead end.
            chain_from[index] = 1;
            return 1;
        }
        chain_from[index] = 0;  // initialize to 0 so we don't loop forever before recursing a cycle
        int sub_chain = populateChain(nums, nums[index]);
        chain_from[index] = sub_chain + 1;
        return chain_from[index];
    }
public:
    int arrayNesting(vector<int>& nums) {
        int best=0;
        for (int k=0; k<nums.size(); k++) {
            int candidate = populateChain(nums, k);
            best = std::max(best, candidate);
        }
        return best;
    }
};
