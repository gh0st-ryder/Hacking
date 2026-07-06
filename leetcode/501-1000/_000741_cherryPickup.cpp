class Solution {
    // This is a very nice twist on the standard DP problem.
    //
    // There are a couple of different issues to consider right off the bat, so let's discuss them.
    //
    // Issue 1:
    // Note that there are two sub-problems:
    // Move from (0, 0) to (n-1, n-1), then move from (n-1, n-1) to (0, 0)
    // The first part is only using (D, R), the second part is only using (U, L)
    //     Note that these two are symmetrical in the following sense: 
    //     If I give you an optimal path from (n-1, n-1) to (0, 0) using only (U, L)
    //     then I can reverse that EXACT path, go from (0, 0) to (n-1, n-1) using only (D, R)
    //     and end up picking the same optimal number of cherries!
    // Therefore our given problem reduces to running (0, 0) to (n-1, n-1) twice.
    //
    // Issue 2:
    // This is the bigger issue.
    // Given our conclusion above, can I solve the original problem twice, just mutating the grid state in between runs?
    // It is unclear if that is the optimal solution
    // Consider these two runs:
    //     Run 1:
    //     Solve optimally using DP => gives N11 cherries
    //     mutate grid state by removing the cherries we picked
    //     Solve optimally using DP => gives N12 cherries
    //     Run 2:
    //     Solve SUB-optimally => gives N21 cherries
    //     mutate grid state by removing the cherries we picked
    //     Solve SUB-optimally => gives N22 cherries
    //
    // The crux of the question is:
    // Can we prove that N11 + N12 >= N21 + N22
    // If so, then we can just run the DP algorithm twice, with mutation in between.
    // If not, then we have a problem, because we need to devise a sub-optimal algorithm,
    // which when run twice with mutation in between, results in a larger total.
    // What might this be?
    //
    // We can in fact come up with a counter example that shows running DP back to back twice 
    // and adding up the results will NOT work.
    //
    // Since we have two traversals, we have to do both simultaneously
    // The memoized state depends on vars (r1, c1) and (r2, c2)
    // We can drop one state var by considering c2 = r1 + c1 - r2
    // since: (r1+c1) == (r2+c2) in lockstep.
    //
    // The recurrence relation is as follows:
    //
    // solve(n-1, n-1, n-1) = 0  => base case
    //
    // solve(r1, c1, r2) = {max of =>
    //     solve(r1+1, c1, r2+1) + NUM1 + NUM2  => both move D
    //     solve(r1, c1+1, r2)   + NUM1 + NUM2  => both move R
    //     solve(r1+1, c1, r2)   + NUM1 + NUM2  => 1 moves D, 2 moves R
    //     solve(r1, c1+1, r2+1) + NUM1 + NUM2  => 1 moves R, 2 moves D
    // }    
    // where NUM1/2 = zero or one cherry picked up in this step by each of 1 or 2
    //                accounting for the fact that if they both end up in the same cell
    //                and it has a cherry, we only count it once.
    // also account for out-of-bounds indices above and drop them if needed.

    vector<vector<int>> grid;
    int N=0;
    bool reached = false;  // did we reach the end?

    // For memoization.
    unordered_map<int, int> cache;
    int makeKey(int r1, int c1, int r2) {
        return ((r1) | (c1 << 8) | (r2 << 16));        
    }

    int solve(int r1, int c1, int r2) {
        if (r1 == N-1 && c1 == N-1 && r2 == N-1) {
            reached = true;
            return 0;  // terminating case
        }

        int key = makeKey(r1, c1, r2);
        if (cache.find(key) != cache.end()) return cache[key];

        int ans = std::numeric_limits<int>::min();
        vector<vector<int>> deltas = {
            // dr1, dc1, dr2
            {1, 0, 1},
            {0, 1, 0},
            {1, 0, 0},
            {0, 1, 1},
        };
        for (const auto& ds : deltas) {
            // Compute the new coordinates.
            int nr1 = r1+ds[0];
            int nc1 = c1+ds[1];
            int nr2 = r2+ds[2];
            int nc2 = nr1 + nc1 - nr2;

            // Throw out what doesn't work.
            if (nr1 == N || nr2 == N || nc1 == N || nc2 == N) continue;
            if (grid[nr1][nc1] == -1) continue;  // this position of person 1 is not possible
            if (grid[nr2][nc2] == -1) continue;  // this position of person 2 is not possible

            // Compute candies picked in this step.
            int p1 = grid[nr1][nc1];  // person p1 picks this candy if it exists
            int p2 = grid[nr2][nc2];  // person p2 picks this candy if it exists
            int correction = ((p1 == 1 && nr1 == nr2 && nc1 == nc2) ? -1 : 0);  // correct the double pick.
            int picked = p1 + p2 + correction;

            // Compute recursive picks + this step, and adjust max.
            int my_total = solve(nr1, nc1, nr2) + picked;
            ans = std::max(ans, my_total);
        }
        cache[key] = ans;
        return ans;
    }
public:
    int cherryPickup(vector<vector<int>>& grid_in) {
        grid = grid_in;
        N = grid.size();
        int ans = solve(0, 0, 0) + (grid[0][0] == 1 ? 1 : 0);
        if (!reached) return 0;
        return ans;
    }
};
