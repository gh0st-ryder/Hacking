class Solution {
    // 
    // 13/21 cases passed, rest TLE
    // Even with path pruning, DFS isn't enough
    // Need a better approach
    //
    double best = 0;
    unordered_map<int, vector<std::pair<int, double>>> adj_list;

    // Key: node; Value: the best multiple reached so far from start to this node
    // If we encounter a lower multiple, we don't recurse
    unordered_map<int, double> best_so_far;

    // me is the node that dfs is called on, end is the end node, val is the accumulated multiple so far.
    void dfs(int me, int end, double val, int came_from, int depth) {
        if (me == end) {
            best = std::max(best, val);
            return;
        }

        // Pruning.
        if (depth >= adj_list.size()) return;
        
        for (const auto& neigh : adj_list[me]) {
            if (neigh.first == came_from) continue;
            double mult = val * neigh.second;
            if (best_so_far.find(neigh.first) == best_so_far.end() || best_so_far[neigh.first] <= mult) { // Pruning
                best_so_far[neigh.first] = mult;
                dfs(neigh.first, end, mult, me, depth+1);
            }
        }
    }
public:
    double maxProbability(int n, vector<vector<int>>& edges, vector<double>& succProb, int start_node, int end_node) {
        for (int i=0; i<edges.size(); i++) {
            int a=edges[i][0], b=edges[i][1];
            adj_list[a].push_back(make_pair(b, succProb[i]));
            adj_list[b].push_back(make_pair(a, succProb[i]));
        }
        for (int i=0; i < adj_list.size(); i++) {
            std::sort(adj_list[i].begin(), adj_list[i].end(), [](const std::pair<int, double>& lhs, const std::pair<int, double>& rhs) -> bool {
                if (lhs.second != rhs.second) return lhs.second > rhs.second;
                return lhs.first < rhs.first;
            });
        }
        dfs(start_node, end_node, 1.0, -1, 0);
        return best;
    }
};
