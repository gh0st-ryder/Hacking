class Solution {
    unordered_map<int, unordered_map<int, bool>> adj_list;

    int dfs(int node, int came_from) {
        int count=0;
        for (const auto& kv : adj_list[node]) {
            if (kv.first == came_from) continue;  // skip the parent that called me
            if (kv.second == false) {                
                count++;
            }
            count += dfs(kv.first, node);
        }
        return count;
    }

public:
    int minReorder(int n, vector<vector<int>>& connections) {
        // Construct the adjacency list.
        for (const vector<int>& edge : connections) {
            adj_list[edge[0]].insert({edge[1], false});
            adj_list[edge[1]].insert({edge[0], true});
        }

        return dfs(0, -1);
    }
};
