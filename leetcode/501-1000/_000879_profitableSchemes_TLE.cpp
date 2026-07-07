class Solution {
    //
    // Let's think of the brute force solution first:
    // A vector of length L, gives us 2^L possible groupings (powerset)
    // We can run through these groupings and filter those that fail one of the following:
    //   1. The count of all the people involved > n
    //   2. The total profit of all the groupings involved < minProfit
    // 
    // The problem with this approach is two-fold:
    //   1. We generate candidates, and then filter them. We are doing needless work.
    //      It would be nice if we didn't generate the invalid ones in the first place.
    //   2. Even with (1) fulfilled, the question says we need to produce the answer
    //      modulo (10^9 + 7). This means there are still way too many combinations.
    // Enumerating them is NOT going to work out. We need some way to "count" them without enumeration.
    //
    // Let's try to formulate an answer with the "take-it/drop-it" paradigm.
    // 
    // Let's consider index 0. We have 2 options.
    // Take it: The group/profit at index 0 is part of our solutions
    //          In this case, the number of combinations is found by 
    //          solving the subproblem from [1..end] with (n-group[0]) and (minProfit-profit[0])
    // Drop it: This group/profit is NOT part of our solutions.
    //          In this case, the number of combinations is found by
    //          solving the subproblem from [1..end] with (n) and (minProfit)
    //
    // So it seems our DP state is dependent on 3 things:
    // 1. The array index i
    // 2. The value of n' at that subproblem.
    // 3. The value of minProfit' at that subproblem.
    //

    vector<int> group, profit;
    const long long MODULO = 1000000007;

    // The memoization cache.
    unordered_map<int, int> cache;
    int make_key(int index, int np, int mpp) {
        return ((index) | (np << 8) | (mpp << 16));
    }

    // Takes an index, n prime, and min_profit prime.
    // The index is the index of the step we will consider.
    // n_p = n' = n - (number of people picked by prev calls)
    // min_profit_p = min_profit' = the amount left to be satisfied, before we hit min_profit (or the surplus if negative)
    long long ways(int index, int n_p, int min_profit_p) {
        if (index >= group.size()) {
            // We ran out of assignments: 
            // the fact that we got here means that we are within our "n" people constraint
            // check if we met our min_profit constraint also
            // here min_profit_p represents the remainder of profit we need to satisfy (or the surplus if negative)
            return (min_profit_p > 0 ? 0 : 1);
        }

        int key = make_key(index, n_p, min_profit_p);
        if (cache.find(key) != cache.end()) return cache[key];

        long long my_ways = 0;
        // Compute how many ways for me
        if (n_p >= group[index]) {
            // take index
            int rec_n_p = n_p - group[index];
            int rec_min_profit_p = min_profit_p - profit[index];
            // Let's clamp this to 0 for optimization, so all negative numbers explore the same state key in the map.
            rec_min_profit_p = std::max(0, rec_min_profit_p);
            long long rec_ways = ways(index + 1, rec_n_p, rec_min_profit_p);
            my_ways += rec_ways;
            my_ways %= MODULO;
        }
        {
            // drop index
            long long rec_ways = ways(index+1, n_p, min_profit_p);
            my_ways += rec_ways;
            my_ways %= MODULO;
        }
        cache[key] = my_ways;
        return my_ways;
    }
public:
    int profitableSchemes(int n, int minProfit, vector<int>& group_in, vector<int>& profit_in) {
        group = group_in;
        profit = profit_in;
        long long ans = ways(0, n, minProfit);
        return ans;
    }
};
