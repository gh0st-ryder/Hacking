class Solution {
public:
    string pushDominoes(string dominoes) {
        set<int> moves;
        vector<int> tick_concluded(dominoes.size(), 0);   // the tick in which domino at this index fell
        int tick=1;  // starting the clock at 1

        for (int i=0; i<dominoes.size(); i++) {
            if (dominoes[i] != '.') {
                moves.insert(i);  // the i-th index moves in the next tick
                tick_concluded[i] = tick;
            }
        }


        set<int> new_moves; // new moves encountered in this tick

        while (true) {
            if (moves.size() == 0) break;  //  no new moves

            for (int index : moves) {   
                // printf("Examine domino at index %d with state %c\n", index, dominoes[index]);             
                if (dominoes[index] == 'L' && tick_concluded[index] == tick) {
                    // printf("Tick %d: domino %d fell L\n", tick, index);
                    // tick_concluded[index] = tick;
                    int new_index = index-1;
                    if (new_index >= 0 && (tick_concluded[new_index] == 0 || tick_concluded[new_index] == tick+1)) { 
                        if (dominoes[new_index] == 'R') {
                            dominoes[new_index] = '.';
                            new_moves.erase(new_index); // get hit by both L and R next tick
                            tick_concluded[new_index] = 0;
                        } else {
                            dominoes[new_index] = 'L';
                            tick_concluded[new_index] = tick+1;
                            new_moves.insert(new_index);
                        }
                    }
                } else if (dominoes[index] == 'R' && tick_concluded[index] == tick) {
                    // printf("Tick %d: domino %d fell R\n", tick, index);
                    // tick_concluded[index] = tick;
                    int new_index = index+1;
                    if (new_index < dominoes.size() && (tick_concluded[new_index] == 0 || tick_concluded[new_index] == tick+1)) {
                        if (dominoes[new_index] == 'L') {
                            dominoes[new_index] = '.';
                            new_moves.erase(new_index); // get hit by both L and R next tick
                            tick_concluded[new_index] = 0;
                        } else {
                            dominoes[new_index] = 'R';
                            tick_concluded[new_index] = tick+1;
                            new_moves.insert(new_index);
                        }                        
                    }
                } 
            }

            // Update simulation.
            tick++;
            moves = new_moves;
            new_moves.clear();
            // printf("moves.size()=%d\n", moves.size());
        }
        return dominoes;
    }
};
