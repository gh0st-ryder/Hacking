struct Edge {
    int a=0;
    int b=0;
    bool operator==(const Edge& rhs) const {
        return a==rhs.a && b==rhs.b;
    }
};
template<>
struct hash<Edge> {
    size_t operator()(const Edge& e) const {
        return ((e.a) | (e.b << 16));
    }
};

class Solution {
    // adjacency list
    unordered_map<int, unordered_set<int>> adj_list;

    // stack will contain the nodes in a cycle
    vector<int> stack;
    unordered_set<int> visited;
    
    void buildAdjList(const vector<vector<int>>& edges) {
        for (const vector<int>& edge : edges) {
            adj_list[edge[0]].insert(edge[1]);
            adj_list[edge[1]].insert(edge[0]);
        }
    }

    // Will DFS into the graph and break when it detects a cycle.
    bool getCycle(int node, int came_from) {
        if (visited.find(node) != visited.end()) {
            // detected a cycle
            stack.push_back(node);  // add it again
            return true;
        }
        visited.insert(node);
        stack.push_back(node);
        for (int neigh : adj_list[node]) {
            if (came_from == neigh)  continue;
            if (getCycle(neigh, node)) return true;
        }
        visited.erase(node);
        stack.pop_back();
        return false;
    }

    // prune will remove all numbers from the start until it encounters the end number the first time
    void prune() {
        vector<int> pruned;
        int check = stack.back();
        int index=0;
        for (; index < stack.size(); index++) {
            if (stack[index] == check) {                
                break;
            }
        }
        for (; index < stack.size(); index++) {
            pruned.push_back(stack[index]);
        }
        stack = pruned;
    }

    Edge edgeToRemove(const vector<vector<int>>& edges) {
        unordered_set<Edge> edgeset;
        for (int i=0; i<stack.size()-1; i++) {
            Edge e;
            e.a = std::min(stack[i], stack[i+1]);
            e.b = std::max(stack[i], stack[i+1]);
            edgeset.insert(e);
        }

        // check in order backwards, which edge to remove
        for (int i=edges.size()-1; i>=0; i--) {
            Edge check = {edges[i][0], edges[i][1]};
            if (edgeset.find(check) != edgeset.end()) {
                return check;
            }
        }
        // should not get here
        assert(0);
        return {};
    }
public:
    vector<int> findRedundantConnection(vector<vector<int>>& edges) {
        buildAdjList(edges);
        assert(getCycle(1, -1));  // should return true
        prune();
        Edge remove = edgeToRemove(edges);
        vector<int> ret;
        ret.push_back(remove.a);
        ret.push_back(remove.b);
        return ret;
    }
};
