class Solution {
    enum State {
        UNSURE=0,
        SAFE=1,
        UNSAFE=2
    };
    vector<vector<int>> graph;    
    
    // Mark nodes unsure, safe, unsafe.
    unordered_map<int, State> states;

    // To track dfs and any cycle we encounter.
    unordered_set<int> visited;

    // Determines if node "node" is a safe node by performing a DFS.
    bool isSafe(int node) {
        if (states[node] == SAFE) return true;
        if (states[node] == UNSAFE) return false;

        if (graph[node].size() == 0) {
            // This is a terminal node.
            states[node] = SAFE;
            return true;
        }
        if (visited.find(node) != visited.end()) {
            // Detected a cycle, not safe
            states[node] = UNSAFE;
            return false;
        }

        visited.insert(node);  // mark        
        bool safe = true;
        for (int neigh : graph[node]) {
            if (!isSafe(neigh)) {
                safe = false;
                break;
            }
        }
        if (safe) {
            states[node] = SAFE;
        } else {
            states[node] = UNSAFE;
        }
        visited.erase(node);  // unmark        
        return safe;
    }
public:
    vector<int> eventualSafeNodes(vector<vector<int>>& graph_in) {
        graph = graph_in;
        for (int i=0; i<graph_in.size(); i++) {
            states[i] = UNSURE;
        }
        for (int i=0; i<graph_in.size(); i++) {
            if (states[i] == UNSURE) {
                isSafe(i);
            }
        }
        vector<int> result;
        for (const auto& kv : states) {
            if (kv.second == SAFE) {
                result.push_back(kv.first);
            }
        }
        std::sort(result.begin(), result.end());
        return result;
    }
};
