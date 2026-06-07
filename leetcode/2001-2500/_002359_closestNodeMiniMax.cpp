class Solution {
    unordered_map<int, unordered_set<int>> adj_list;

    struct Info {
        int node=0;  // the new node reached
        int dist=0;  // distance from the original node
    };

    // Performs a BFS to find the nodes reachable and min hops to get there.
    unordered_map<int, int> getReachable(int from) {
        unordered_map<int, int> ans;
        deque<Info> mq;
        ans[from] = 0;
        mq.push_back({from, 0});
        while (!mq.empty()) {
            Info info = mq.front();
            mq.pop_front();            

            for (int neigh : adj_list[info.node]) {
                if (ans.find(neigh) != ans.end()) continue;  // already processed
                ans[neigh] = info.dist + 1;
                mq.push_back({neigh, ans[neigh]});
            }
        }
        return ans;
    }
public:
    int closestMeetingNode(vector<int>& edges, int node1, int node2) {
        for (int i=0; i<edges.size(); i++) {
            adj_list[i].insert(edges[i]);
        }
        unordered_map<int, int> ans1 = getReachable(node1);
        unordered_map<int, int> ans2 = getReachable(node2);
        vector<int> cand_nodes;
        int minimax=std::numeric_limits<int>::max();
        for (const auto& kv : ans1) {
            if (ans2.find(kv.first) == ans2.end()) continue;
            int my_max = std::max(kv.second, ans2[kv.first]);
            if (my_max < minimax) {
                minimax = my_max;
                cand_nodes.clear();
                cand_nodes.push_back(kv.first);
            } else if (my_max == minimax) {
                cand_nodes.push_back(kv.first);
            }
        }
        if (cand_nodes.size() == 0) return -1;
        auto ans_it = std::min_element(cand_nodes.begin(), cand_nodes.end());
        return *ans_it;
    }
};
