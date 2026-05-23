class Solution {

public:

    unordered_map<string, bool> memo;
    
    bool canWinMemo(string& seq) {
        if (memo.find(seq) != memo.end()) {
            return memo[seq];
        }
        bool ans = true, done = false;
        for (int i=0; i<seq.size()-1 && !done; i++) {
            if (!(seq[i] == '+' && seq[i+1] == '+')) continue;  // can't flip this combo

            // Do the flip
            seq[i] = seq[i+1] = '-';

            bool eval = canWinMemo(seq);

            // Undo the flip
            seq[i] = seq[i+1] = '+';

            // at least one flip I made let me win
            if (eval == false) {
                memo[seq] = true;
                return true;
            }
        }

        memo[seq] = false;
        return false;
    }
    bool canWin(string currentState) {        
        return canWinMemo(currentState);
    }
};
