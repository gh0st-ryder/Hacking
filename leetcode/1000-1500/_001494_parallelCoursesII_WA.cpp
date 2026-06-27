class Solution {
    // The following is a good attempt, but greedy does NOT work here
    // Some test cases fail
    //
    // For each course, we will compute the following numbers:
    // earliest: The earliest semester that it can be taken, assuming k is infinite.
    // long_chain: The longest chain of dependents leading to a sink.
    // 
    // We will sort all courses based first on earliest, and next on long_chain
    //
    // Then we will walk down this list, trying to pack up to k courses into a semester
    // The semester rolls over when either courses == k or,
    // when the next course can't be accomodated in the current semester based on the "earliest" value.

    unordered_map<int, unordered_set<int>> adj_list;
    unordered_map<int, unordered_set<int>> rev_adj_list;  // the reverse adjacency list
    unordered_map<int, int> count_incoming;
    unordered_map<int, int> rev_count_incoming;  // the count_incoming for the reverse adjacency list

    // Keeps tab of the earliest semester I could take a course, assuming k is infinite.
    // key: course, value: the earliest semester I can take this course
    unordered_map<int, int> earliest;
    // Key: course, value: the longest chain from this key leading to a sink node
    unordered_map<int, int> long_chain;

    void compute_adj_list(const vector<vector<int>>& relations) {        
        for (const vector<int>& relation: relations) {
            adj_list[relation[0]].insert(relation[1]);
            count_incoming[relation[1]] += 1;
        }
    }

    void compute_rev_adj_list(const vector<vector<int>>& relations) {        
        for (const vector<int>& relation: relations) {
            rev_adj_list[relation[1]].insert(relation[0]);
            rev_count_incoming[relation[0]] += 1;
        }
    }
    
    // Inputs are:
    // adj_list (or rev_adj_list)
    // count_incoming (or rev_count_incoming)
    // Output is:
    // earliest (or long_chain)
    void compute_result(unordered_map<int, unordered_set<int>>& adj, unordered_map<int, int>& count, unordered_map<int, int>& result) {
        // printf("---------------------------------\n");
        // first: course, second: depth
        deque<pair<int, int>> mq;
        // Find all the sources (no incoming edges) [or sinks (no outgoing edges)].
        for (const auto& kv : adj) {
            if (count.find(kv.first) == count.end()) {
                mq.push_back({kv.first, 1});
            }
        }

        // Perform a multi-source (or multi-sink) BFS.        
        unordered_map<int, int> prereq_done;  // keeps a count of number of prereqs done for this key

        while (!mq.empty()) {
            auto fs= mq.front();
            mq.pop_front();       
            int course = fs.first;
            int depth = fs.second;
            result[course] = depth;
            // printf("result[%d] = %d\n", course, depth);

            // Add not taken courses
            for (int neigh : adj[course]) {                
                // First mark prereqs for dependent courses
                prereq_done[neigh] += 1;  // taking course "course" caused this
                
                if (prereq_done[neigh] == count[neigh]) {                                        
                    mq.push_back({neigh, depth+1});
                }
            }
        }
        // printf("---------------------------------\n");
    }

  public:
    int minNumberOfSemesters(int n, vector<vector<int>>& relations, int k) {
        compute_adj_list(relations);
        compute_rev_adj_list(relations);
        compute_result(adj_list, count_incoming, earliest);
        compute_result(rev_adj_list, rev_count_incoming, long_chain);
        
        vector<int> courses;
        for (int i=1; i<=n; i++) {courses.push_back(i);}

        std::sort(courses.begin(), courses.end(), [this](int lhs, int rhs) -> bool {
            if (earliest[lhs] != earliest[rhs]) return earliest[lhs] < earliest[rhs];
            if (long_chain[lhs] != long_chain[rhs]) return long_chain[lhs] > long_chain[rhs];
            return lhs < rhs;  // tie break 
        });

        int semesters=0;
        int running_count=0;  // keeps track of how many we packed into this semester so far
        int offset=0;  // an offset is caused by the rolling over problem, which affects the earliest values considered
        for (int course : courses) {
            printf("Considering course %d\n", course);
            if (earliest[course] + offset > semesters) {                
                // We start a new semester
                semesters = earliest[course] + offset;
                running_count = 1;  // start packing again
                printf("Start a new semester %d due to course restriction\n", semesters);
            } else if (running_count == 0) {
                // We start a new semester
                semesters += 1;
                running_count = 1;  // start packing again
                offset++;
                printf("Start a new semester %d due to overflow\n", semesters);
            } else {
                printf("Packed it into the current semester %d\n", semesters);
                running_count++;  // packed another course into this semester
                running_count %= k;
            }
        }
        return semesters;
    }
};

