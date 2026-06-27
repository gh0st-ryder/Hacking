class Solution {
    // This problem has 3 distinct sub-cases:
    // 1. Cycle of length >=3 (e.g. A -> B -> C -> D -> A)
    // 2. Cycle of length 2, where both chains feed into each other (e.g. A->B->C<->R<-Q<-P)
    // 3. Disjoint bunches of 2s, with or without (2) above (e.g. A<->B C<->D E<->F G->H->I<->J<-K<-L)

    //
    // For the sub-problem 1, we can use the following logic:
    //
    // Imagine we pick some person and seat them at the table.
    // Now on their right, we HAVE to seat their favorite
    // And on that persons's right we HAVE to seat their favorite, and so on.
    // This continues until one of two things happens:
    // The rightmost person's favorite is the starting person: The chain is COMPLETED.
    // The rightmost person's favorite is a person already part of the chain that has gone before: The chain is TRUNCATED.
    //
    // If the chain is completed, then for each person in the chain, for this arrangement, they are involved in a length-n chain.
    // If the chain is truncated, then two things happen:
    //    1. Replace the starting person with the truncated person and consider the smaller chain completed (outcome = chain length for each).
    //    2. All people from the starting person to the truncated-1 person should NOT be part of the table ever (outcome = 0 for each).
    //
    // Therefore for each of the person's that we have explored in the above completed or truncated chain, we have an outcome.
    //
    // Now we iterate through the remaining unexplored persons. 
    // We pick the next starting person, and repeat.
    // At the end we have an "outcome" number for each person.
    // We pick the biggest "outcome" number for the answer to the problem.
    //

    // To solve sub-problems 2&3 we can construct a reverse edge graph.
    // Then find all the pairs of mutual-favorites (e.g. A<->B)
    // Then find the longest non-chain ending in A, find the longest non-chain ending in B (may be just length 1)
    // Then add all such lengths

    // The final answer is the max of subproblem 1 or the addition of the lengths above.

    // index: person; value: all the people that list person as favorite
    vector<unordered_set<int>> is_favorite;
    vector<int> favorite;
    vector<int> outcome;
    int best=0; // for subproblem 1
    int sum_of_twos=0;  // for subproblems 2 and 3.

    void chainalysis(int start, int end) {
        int count=1;
        int me=start;
        while (me != end) {
            me = favorite[me];
            count++;            
        }
        // printf("Chain length %d from %d -> %d\n", count, start, end);
        me = start;
        while (me != end) {
            outcome[me] = count;
            me = favorite[me];
        }
        outcome[end] = count;        
        best = std::max(best, count);
    }

    // From from to just before after_to, mark outcomes to be 0.
    void chaininvalid(int from, int after_to) {
        while (from != after_to) {
            outcome[from] = 0;
            from = favorite[from];
        }
    }

    void explore(int person) {
        int start=person;
        unordered_set<int> explored;        
        while (explored.find(favorite[person]) == explored.end()) {
            explored.insert(person);
            person = favorite[person];            
            if (outcome[person] != -1) {
                // Will lead to an invalid initial sequence.
                chaininvalid(start, person);
                return;
            }            
        }
        chainalysis(favorite[person], person);
        chaininvalid(start, favorite[person]);
    }

    int dfs(int node, int exclude) {
        int biggest = 0;
        for (int neigh : is_favorite[node]) {
            if (neigh == exclude) continue;
            int my_len = dfs(neigh, exclude);
            biggest = std::max(biggest, my_len);
        }
        return 1 + biggest;
    }

    // Solve subproblems 2&3 for mutual lovers a<->b
    // Find the longest non-cyclic chain leading into a
    // Find the longest non-cyclic chain leading into b
    // Add those two lengths
    // update sum_of_twos 
    void subproblem23(int a, int b) {
        int lena = dfs(a, b);
        int lenb = dfs(b, a);
        int my_sum = lena + lenb;
        sum_of_twos += my_sum;
    }

public:
    int maximumInvitations(vector<int>& favorite_in) {
        favorite = favorite_in;
        outcome = vector<int>(favorite.size(), -1);
        is_favorite = vector<unordered_set<int>>(favorite.size(), unordered_set<int>{});

        for (int i=0; i<favorite.size(); i++) {
            is_favorite[favorite[i]].insert(i);
        }

        for (int i=0; i<favorite.size(); i++) {
            if (outcome[i] == -1) {                
                explore(i);
            }
        }

        // Process mutual lovers.
        for (int i=0; i<favorite.size(); i++) {
            if (favorite[favorite[i]] == i && i < favorite[i]) { // the less than avoids double counting (a, b vs. b, a will both match)
                subproblem23(i, favorite[i]);
            }
        }
        return std::max(best, sum_of_twos);
    }
};
