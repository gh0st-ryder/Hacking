class Solution {
    // We sort the collection on startTime, and start processing backwards.
    // To break ties, we sort on endTime, then on profit
    //
    // Let maxProfit(t) denote the maximum profit obtained by solving the problem for time t or later
    // maxprofit(earliest_start_time) will give us our answer
    //
    // The recurrence relation is (for item index i with starttime t):
    // maxProfit(t) = max of 1 or 2 where
    //   1 => consider that we take item i,     
    //        = profit[i] + maxProfit(time) ;  time >= item[i].endTime
    //   2 => consider that we drop item i,
    //        so we find maxProfit(time) ; time >= item[i].startTime
    //
    struct Element {
        int startTime;
        int endTime;
        int profit;
    };

    // Key: time t
    // Value: the max profit solving the problem starting at time t
    map<int, int> time_to_profit;
public:
    int jobScheduling(vector<int>& startTime, vector<int>& endTime, vector<int>& profit) {
        vector<Element> elems;
        int largest_end_time=0, earliest_start_time=std::numeric_limits<int>::max();
        for (int i=0; i<startTime.size(); i++) {
            elems.push_back({startTime[i], endTime[i], profit[i]});
            largest_end_time = std::max(largest_end_time, endTime[i]);
            earliest_start_time = std::min(earliest_start_time, startTime[i]);
        }

	// O(n log n) for sort.
        std::sort(elems.begin(), elems.end(), [](const Element& lhs, const Element& rhs)->bool {
            if (lhs.startTime != rhs.startTime) return lhs.startTime < rhs.startTime;
            if (lhs.endTime != rhs.endTime) return lhs.endTime < rhs.endTime;
            return lhs.profit < rhs.profit;            
        });

        // Base case:
        time_to_profit[largest_end_time] = 0;

        // DP cases:
	// O(n log n) since lower_bound is O(log n).
        for (int i=elems.size()-1; i>=0; i--) {            
            // Take item i
            const auto& take_it = time_to_profit.lower_bound(elems[i].endTime);
            int take_profit = elems[i].profit + take_it->second;

            // Drop item i
            const auto& drop_it = time_to_profit.lower_bound(elems[i].startTime);
            int drop_profit = drop_it->second;

            // The best is one of take, drop (or another item that already recorded this key, which is found by drop_it already).
            int best = std::max(take_profit, drop_profit);
            time_to_profit[elems[i].startTime] = best;
        }

        return time_to_profit[earliest_start_time];
    }
};
