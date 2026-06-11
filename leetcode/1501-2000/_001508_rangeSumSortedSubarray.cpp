class Solution {

    const long long DENO = 1000000007;
    
public:

    int rangeSum(vector<int>& nums, int n, int left, int right) {        
        vector<int> sums(nums.size(), 0);
        sums[0] = nums[0];
        for (int i=1; i<nums.size(); i++) {
            sums[i] = sums[i-1] + nums[i];
        }

        
        int combos_size = n*(n+1)/2;
        int remove=0;
        vector<int> combos(combos_size, 0);
        int combo_index=0;
        for (int i=0; i<nums.size(); i++) {
            int my_combos = n-i;
            for (int mc=0; mc<my_combos; mc++, combo_index++) {
                combos[combo_index] = sums[i + mc] - remove; 
            }
            remove += nums[i];
        }
        std::sort(combos.begin(), combos.end());

        long long ans=0;
        int left0=left-1, right0=right-1;
        for (int mc=left0; mc<=right0; mc++) {            
            ans += combos[mc];
            ans %= DENO;
        }
        return ans;
    }
};
