class Solution {
    // For each index we will maintain a Result.
    struct Result {
        int winner;  // 0=Tie, 1=Alice, 2=Bob (assumes Alice plays at this index).
        int value;   // The max optimal value reached at this index.
        int pick;    // For debug: The num of stones picked at this index
        int total;   // The total of all the stones so far (computed while iterating backwards)
    };

    string answer(int winner) {
        if (winner == 0) return "Tie";
        if (winner == 1) return "Alice";
        if (winner == 2) return "Bob";
        return "Mistake";  // this should not happen
    }

    vector<Result> dp;

    // We will iterate backwards
    // At any point, let total be the total[i] of all the stones seen so far at that index i (backwards)
    // The recurrence relation is:
    // dp[i].value = max(a, b, c) where
    //   a = stoneValue[i] + dp[i+1].total - dp[i+1].value;                                      // pick 1 stone
    //   b = stoneValue[i] + stoneValue[i+1] + dp[i+2].total - dp[i+2].value;                    // pick 2 stones
    //   c = stoneValue[i] + stoneValue[i+1] + stoneValue[i+2] + dp[i+3].total - dp[i+3].value;  // pick 3 stones
public:
    string stoneGameIII(vector<int>& stoneValue) {
        int N = stoneValue.size();
        dp = vector<Result>(N, {0, 0, 0, 0});
        
        // Base cases:       
        { 
            // N-1 case
            int win_nm1 = (stoneValue[N-1] < 0 ? 2 : (stoneValue[N-1] == 0 ? 0 : 1));
            dp[N-1] = {win_nm1, stoneValue[N-1], 1, stoneValue[N-1]};
            if (N == 1) return answer(dp[N-1].winner);
        }
        {
            // N-2 case
            int a = stoneValue[N-2] + dp[N-1].total - dp[N-1].value;
            int b = stoneValue[N-2] + stoneValue[N-1];
            int total = b;
            if (b >= a) {  // Pick b (both stones).                
                int win_nm2 = (b < 0 ? 2 : (b == 0 ? 0 : 1));
                dp[N-2] = {win_nm2, b, 2, b};
            } else {  // Pick a (only the first stone).
                int win_nm2 = (a < 0 ? 2 : (a == 0 ? 0 : 1));
                dp[N-2] = {win_nm2, a, 1, b};
            }
            if (N == 2) return answer(dp[N-2].winner);
        }
        {
            // N-3 case
            int a = stoneValue[N-3] + dp[N-2].total - dp[N-2].value;
            int b = stoneValue[N-3] + stoneValue[N-2] + dp[N-1].total - dp[N-1].value;
            int c = stoneValue[N-3] + stoneValue[N-2] + stoneValue[N-1];            
            int total = c;
            int my_max = std::max(c, std::max(a, b));
            int other = total - my_max;
            int win_nm3 = (my_max == other ? 0 : my_max > other ? 1 : 2);
            if (c == my_max) {  // Pick all 3 stones.
                dp[N-3] = {win_nm3, c, 3, c};
            } else if (b == my_max) {  // Pick the first 2 stones.
                dp[N-3] = {win_nm3, b, 2, c};
            } else {  // Pick only the first stone.
                dp[N-3] = {win_nm3, a, 1, c};
            }
            if (N == 3) return answer(dp[N-3].winner);
        }

        // DP cases:
        for (int i=N-4; i>=0; i--) {
            int a = stoneValue[i] + dp[i+1].total - dp[i+1].value;
            int b = stoneValue[i] + stoneValue[i+1] + dp[i+2].total - dp[i+2].value;
            int c = stoneValue[i] + stoneValue[i+1] + stoneValue[i+2] + dp[i+3].total - dp[i+3].value;
            int total = stoneValue[i] + dp[i+1].total;
            int my_max = std::max(c, std::max(a, b));
            int other_score = total - my_max;
            int picks = (my_max == c ? 3 : my_max == b ? 2 : 1);
            int winner = (my_max == other_score ? 0 : my_max > other_score ? 1 : 2);
            dp[i] = {winner, my_max, picks, total};
        }

        return answer(dp[0].winner);
    }
};
