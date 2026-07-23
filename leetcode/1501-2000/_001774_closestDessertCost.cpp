class Solution {
    vector<int> topcosts;
    unordered_set<int> topcostspossible;

    // Given a list of topping costs, calculate all the combinations of topping costs possible.
    // We use the "take-it/drop-it" paradigm to calculate all possible costs and stick those into a hash table.
    // From index i to the end (in topcosts), returns all the possible combinations of costs possible
    // computeTopCosts(0) gives us all possibilities for the problem toppings.
    unordered_set<int> computeTopCosts(int i) {
        if (i == topcosts.size()) return {0};
        unordered_set<int> ret;
        unordered_set<int> rec = computeTopCosts(i+1);
        // Drop index i
        ret = rec;
        // Take index i
        for (int r : rec) {
            ret.insert(r+topcosts[i]);
        }
        return ret;
    }

    // Returns whether it is possible to have a selection of toppings
    // matching exactly the cost toptar.
    bool possible(int toptar) {
        return (topcostspossible.find(toptar) != topcostspossible.end());
    }

public:
    int closestCost(vector<int>& baseCosts, vector<int>& toppingCosts, int target) {
        for (int t : toppingCosts) {
            // Up to 2 possible for each topping.
            topcosts.push_back(t);
            topcosts.push_back(t);
        }
        topcostspossible = computeTopCosts(0);

        for (int tdelta = 0; tdelta <= target; tdelta++) {
            // try target - tdelta
            for (int base : baseCosts) {
                int toptar = target - tdelta - base;
                if (possible(toptar)) {
                    return target - tdelta;
                }
            }
            // try target + tdelta
            for (int base : baseCosts) {
                int toptar = target + tdelta - base;
                if (possible(toptar)) {
                    return target + tdelta;
                }
            }            
        }
        int mindiff = std::numeric_limits<int>::max();
        int best = -1;
        for (int b : baseCosts) {
            int diff = std::abs(b - target);
            if (diff < mindiff) {
                mindiff = diff;
                best = b;
            }
        }
        return best;
    }
};
