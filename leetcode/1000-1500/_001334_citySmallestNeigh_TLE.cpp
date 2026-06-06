
class Solution {
    // DFS causes TLE; I need either Floyd-Warshall's or Dijkstra's algorithms.
    //
    // Outer key: from node
    // Inner key: to node
    // Value: weight
    unordered_map<int, unordered_map<int, int>> adj_list;
    int total_dist=0;

    struct Info {
        int my_node=0;
        int distance_leftover=0;  // this much distance can still be consumed safely to travel
    };

    void buildAdjList(const vector<vector<int>>& edges) {
        for (const vector<int>& edge: edges) {
            adj_list[edge[0]].insert({edge[1], edge[2]});
            adj_list[edge[1]].insert({edge[0], edge[2]});
            total_dist += edge[2];
        }
    }

    void dfs(int node, int threshold, unordered_set<int>& cities, int depth) {    
        // int before_size = cities.size();    
        cities.insert(node);        
        // if (cities.size() == before_size) return;
        if (depth > adj_list.size()) return;  // to prevent needless bouncing around without adding cities

        for (const auto& kv : adj_list[node]) {
            if (kv.second > threshold) continue;
            dfs(kv.first, threshold - kv.second, cities, depth+1);
        }
    }

public:
    int findTheCity(int n, vector<vector<int>>& edges, int distanceThreshold) {
        buildAdjList(edges);
        int chosen=-1, least_num_cities=std::numeric_limits<int>::max();
        int new_threshold = std::min(distanceThreshold, total_dist);
        // printf("The new threshold is %d\n", new_threshold);
        for (int node=n-1; node >=0; node--) {
            unordered_set<int> cities;
            dfs(node, new_threshold, cities, 0);
            int num=cities.size();            
            // printf("From node %d => reachable %d\n", node, num);
            if (num < least_num_cities) {
                chosen = node;
                least_num_cities = num;
            }
        }
        return chosen;
    }
};
