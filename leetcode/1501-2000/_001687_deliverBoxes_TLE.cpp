class Solution {
    //
    // The current solution is O(n^2) and passes 35/39 test cases.
    // Gets a TLE for the rest.
    // need to optimize the inner loop away
    //
    vector<int> ports;
    vector<int> weights;
    vector<int> dp;

    // for any index i, let J be the number of boxes we could accommodate
    // J = min(J1, maxBoxes)
    // J1 = number of boxes from i that add up to maxWeight but not more
    // dp(i) = min across all j [1->J] {
    //     (1 + j) trips + dp(i+j+1)
    // }
    // However we account for consecutive duplicates at the same port by adding 0 trips for that route

public:
    // O(n^2)
    int boxDelivering(vector<vector<int>>& boxes, int portsCount, int maxBoxes, int maxWeight) {
        const int N = boxes.size();        
        for (int i=0; i<boxes.size(); i++) {
            ports.push_back(boxes[i][0]);
            weights.push_back(boxes[i][1]);
        }
        dp = vector<int>(N+1, 0);

        // Base case:
        dp[N] = 0;
        dp[N-1] = 2;

        // DP cases:
        for (int i=N-2; i>=0; i--) {
            int best = std::numeric_limits<int>::max();
            int cum_wt = 0;
            int port_prev = -1, trips=0;
            for (int j=i; j<i+maxBoxes && j < N; j++) {
                cum_wt += weights[j];
                if (cum_wt > maxWeight) break;
                if (ports[j] != port_prev) {
                    trips++;
                }
                port_prev = ports[j];
                int my_count = trips+1 + dp[j+1];
                best = std::min(best, my_count);
            }
            dp[i] = best;
        }
        return dp[0];
    }
};
