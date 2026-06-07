class Solution {
    // Key: id of bomb x
    // Values: ids of all the bombs y that lie within the radius of bomb x.
    // Note that this is not symmetric: y lying within x's blast radius does not imply the reverse necessarily.
    unordered_map<int, unordered_set<int>> adj_list;

    bool withinBlast(double bx, double by, double cx, double cy, double radius) {
        double dist = std::sqrt((bx-cx)*(bx-cx) + (by-cy)*(by-cy));
        bool within = (dist <= radius);
        // printf("Bomb at (%f, %f) (%s) detonates  (%f, %f); dist = %f; radius = %f\n", bx, by, (within?"DOES ":"DOES NOT "), cx, cy, dist, radius);
        return within;
    } 

    // countForDetonation tells us that if we detonate the bomb at "index", how many total transitive detonations we will have.
    int countForDetonation(int index) {
        int count=1;
        deque<int> deq;
        deq.push_back(index);
        unordered_set<int> visited;
        visited.insert(index);

        while (!deq.empty()) {
            int bomb = deq.front();
            deq.pop_front();

            for (const auto& victim : adj_list[bomb]) {
                if (visited.find(victim) != visited.end()) continue;
                visited.insert(victim);
                count++;  // count the victim
                deq.push_back(victim);  // and transitively find how many it touches
            }
        }
        // printf("For index %d, got %d detonations\n", index, count);
        return count;
    }
public:
    int maximumDetonation(vector<vector<int>>& bombs) {
        for (int i=0; i<bombs.size(); i++) {
            int bx=bombs[i][0], by=bombs[i][1], br=bombs[i][2];
            for (int check=0; check<bombs.size(); check++) {
                int cx=bombs[check][0], cy=bombs[check][1];
                if (withinBlast(bx, by, cx, cy, br)) {
                    adj_list[i].insert(check);
                }
            }
        }
        int best=0;
        for (int i=0; i<bombs.size(); i++) {
            int my_count = countForDetonation(i);
            best = std::max(best, my_count);
        }
        return best;
    }
};
