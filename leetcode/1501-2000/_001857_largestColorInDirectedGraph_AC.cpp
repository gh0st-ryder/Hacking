// GOT ACCEPTED FINALLY!
// All test cases pass and the problem is solved!!
class Solution {
    // 
    // If we dfs from every node, then in n^2 time we can explore all possibilities
    // However when we do this, we will revisit certain paths over and over
    // We should memoize the results along paths, and re-use those when we can.
    //
    // Specifically, from each node m, we want to note each chain (and their colors)
    // leading to every other terminal node n (reachable from m)
    //
    // Then later, when another call asks to explore node m as part of another chain,
    // we already have the results to the terminal nodes from m memoized.
    //
    // If at any point we detect a cycle, we can set a class specific "abort" flag, and exit.

    // key = node m
    // values = all nodes n directly having an edge from m to n
    unordered_map<int, unordered_set<int>> adj_list;
    // index = key = node number
    // value = the color of that node
    string colors;
    // the number of the max node
    int max_node = 0;
    // The class abort flag, to stop early and quit with -1.
    bool abort = false;
    // the best result that is returned at the end.
    int best=0;

    // The result for a specific chain from node m to node n, 
    // where node n is a terminal node
    // the results are stored as an array of size 26, storing a count at each of the 26 colors
    struct Result {
        std::array<int, 26> counts;
        Result() {
            for (auto& item : counts) {
                item = 0;
            }
        }
        void recordColor(char color) {
            counts[color - 'a'] += 1;
        }
        // keepbest considers the incoming result and my current result
        // and keeps the best value in my current result for all colors
        void keepBest(const Result& incoming) {
            for (int i=0; i<26; i++) {
                counts[i] = std::max(counts[i], incoming.counts[i]);
            }
        }
    };

    // TermResult stores the Result leading up to the terminal node "key".
    // The "from" node is purposely not specified here because
    // these are "partial" results that are rolled up during recursion unwinding.
    // 
    // Change this from multimap to just an unordered_map
    // And we store the largest number for any color!!!
    // This controls our state explosion, fixing the TLE from last time.
    //
    // Actually it turns out I can skip TermResults entirely and just keep Result
    // just like multiple paths leading to the same terminal node don't matter
    // similarly multiple paths leading to multiple terminal nodes also don't matter!
    // in the end what matter is just the max count for each color from node "node" to SOME terminal path
    // we can collapse all terminal nodes and all paths to all terminal nodes into a single Result!

    // the memoization cache
    // key = "from" node
    // value = Result (which contains the counts for all 26 colors along this path from "from" to "to")
    unordered_map<int, Result> results;

    // checks if best can be updated
    void process(const Result& result) {
        for (int count : result.counts) {
            best = std::max(best, count);
        }
    }

    // visited is kept to detect a cycle
    Result dfs(int node, unordered_set<int>& visited) {
        if (visited.find(node) != visited.end()) {
            abort = true;
            printf("Node %d involved in cycle\n", node);            
            return {};
        }
        // memoize
        if (results.find(node) != results.end()) {
            return results[node];
        }
        
        visited.insert(node);
        Result my_result;
        if (!adj_list[node].empty()) {
            for (int to : adj_list[node]) {
                Result rec = dfs(to, visited);
                if (abort) {return {};}  // early exit                
                // Record my color in the results
                rec.recordColor(colors[node]);
                // and compare and store the best in the result for this node.
                my_result.keepBest(rec);
                // Also count the colors and see if the global needs adjusting
                process(my_result);                
            }
        } else {
            // I am a terminal node!            
            my_result.recordColor(colors[node]);            
            process(my_result);
        }

        // unvisit my node
        visited.erase(node);
        // memoize
        results[node] = my_result;
        return my_result;
       
    }
public:
    int largestPathValue(string colors_in, vector<vector<int>>& edges) {
        colors = colors_in;
        max_node = colors.length()-1;
        for (const auto& edge : edges) {
            adj_list[edge[0]].insert(edge[1]);
        }
        for (int n=0; n<=max_node; n++) {
            unordered_set<int> visited;
            // printf("Trying node %d\n", n);            
            dfs(n, visited);
            // printf("dfs from %d, best is now %d\n", n, best);
            if (abort) return -1;
        }
        return best;
    }
};
