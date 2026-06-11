class Solution {
public:
    int minSubarray(vector<int>& nums, int p) {
        // For index i, the sum of all the nums that come before i.
        vector<long long> prefix_sums;
        long long total=0;
        for (int i=0; i<nums.size(); i++) {
            total += nums[i];
            prefix_sums.push_back(total);            
        }
                        
        if (total % p == 0) return 0;
        int want_modulo = total % p;               

        int best = std::numeric_limits<int>::max();
        // Key: modulo value; Value: rightmost index in prefix_sum with that modulo value
        unordered_map<int, int> rightmost;
        rightmost[0] = -1;  // base case
        for (int right=0; right<prefix_sums.size(); right++) {            
            int my_modulo = prefix_sums[right] % p;            
            int find_modulo = (my_modulo - want_modulo + p)%p;
            if (rightmost.find(find_modulo) != rightmost.end()) {
                int found_index = rightmost[find_modulo];                
                int my_size = right-found_index;
                best = std::min(best, my_size);
            }
            rightmost[my_modulo] = right;
        }
        return best == nums.size() || best == std::numeric_limits<int>::max()? -1 : best;        
    }
};
