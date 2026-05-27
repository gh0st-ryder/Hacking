class Solution {
    // Keep a hashtable of course_id -> set of transitive prereqs
    //
    // Perform a topological sort:
    // Start with a hashtable: course_id -> num_incoming_edges (direct prereq)
    // Put everything with no incoming edge into a deque
    // Pop stuff off the deque one by one
    //     when it is popped, the prereqs are complete
    //     go to all its direct sources, union the transitive prereqs of those, add myself to it, and put that as my prereq set
    //    
    //     for the edges it is leading out to; mark off a direct prereq as done
    //         if the above led to a course having no more direct prereq, put it in the deque    
    //     
    // 
    // Now process the queries one by one
    // For query [u, v], use transitive_prereqs to answer the query

    unordered_map<int, unordered_set<int>> transitive_prereqs;
    unordered_map<int, unordered_set<int>> direct_prereqs;   // for the key -> incoming edges from values to the key
    unordered_map<int, unordered_set<int>> direct_succ;  // for the key -> outgoing edges to values
    
public:
    vector<bool> checkIfPrerequisite(int numCourses, vector<vector<int>>& prerequisites, vector<vector<int>>& queries) {
        for (int i=0; i<numCourses; i++) {
            direct_prereqs[i] = unordered_set<int>();
            transitive_prereqs[i] = unordered_set<int>();
            direct_succ[i] = unordered_set<int>();
        }

        for (const auto& vi: prerequisites) {
            direct_prereqs[vi[1]].insert(vi[0]);
            direct_succ[vi[0]].insert(vi[1]);
        }

        // Key: Course_id, Value: how many direct prereqs have been fulfilled so far
        unordered_map<int, int> fulfilled_direct_prereqs;    
        deque<int> myq;
        for (const auto& kv : direct_prereqs) {
            if (kv.second.empty()) {myq.push_back(kv.first);}
        }

        while (!myq.empty()) {
            int course_id = myq.front(); myq.pop_front();

            // tell my successors that I'm done.            
            for (int succ : direct_succ[course_id]) {
                // First lets add the transitive prereqs of course_id to succ, since we just took course_id
                for (int pre : transitive_prereqs[course_id]) {
                    transitive_prereqs[succ].insert(pre);
                }
                transitive_prereqs[succ].insert(course_id);

                fulfilled_direct_prereqs[succ]++;
                if (fulfilled_direct_prereqs[succ] == direct_prereqs[succ].size()) {
                    // This course is now ready to be taken
                    myq.push_back(succ);                    
                }
            }
        }

        vector<bool> results;
        for (const auto& query : queries) {
            int u = query[0];
            int v = query[1];
            results.push_back(transitive_prereqs[v].find(u) != transitive_prereqs[v].end());
        }
        return results;
    }
};
