class Solution {
    // Key: Index i; Value: Sum of (n-1) numbers from index i to index (i+N)%N
    unordered_map<int, int> nm1_sums;

    // We re-write F(k+1) in terms of F(k) as follows:
    // F(k+1) = F(k) - nums((N-1-k)% N)*(N-1) + nm1_sums[(N-k)%N]
    // 
public:
    int maxRotateFunction(vector<int>& nums) {
        const int N=nums.size();
        int ans=std::numeric_limits<int>::min();
        vector<int> F(N, 0);

        // Compute the base case F(0)
        int sum=0;
        for (int i=0; i<N; i++) {
            F[0] += i*nums[i];            
            if (i != N-1) {
                sum += nums[i];
            }
        }
        ans = std::max(ans, F[0]);
        // printf("F(%d) = %d\n", 0, F[0]);
        nm1_sums[0] = sum;
        // printf("nm1_sums(%d) = %d\n", 0, nm1_sums[0]);

        // Compute the helper hash table.        
        for (int i=N-1; i>0; i--) {
            nm1_sums[i] = nm1_sums[(i+1)%N] + nums[i] - nums[i-1];
            // printf("nm1_sums(%d) = %d\n", i, nm1_sums[i]);
        }

        // Compute the recursive DP cases.
        for (int k=0; k<N-1; k++) {
            F[k+1] = F[k] - nums[N-1-k]*(N-1) + nm1_sums[(N-k)%N];
            ans = std::max(ans, F[k+1]);
            // printf("F(%d) = %d\n", k+1, F[k+1]);
        }
        return ans;
    }
};
