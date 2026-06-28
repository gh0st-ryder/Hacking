class Solution {

    // See note in the else portion below for why this approach leads to WA 
    //
    // We will iterate backwards through the problem.
    // 
    // At each index i, we have the choice to keep the letter, or drop the letter

    // We will maintain this info for each DP cell.
    struct Info {
        int val;  // the value of the optimal solution at this index (rle_encoded string length)
        char ch;  // the char to start this optimal solution at this index (may be from later ones if we chose to drop)
        int num;  // the length of the string of same char at the start at this index        
    };

    // The cache for top-down DP.
    vector<vector<Info>> dp;

    // returns the rle encoding for the given string.
    string rle_encoded(const string& str) {
        string rle;
        for (int index=0; index <str.size(); ) {
            int len = count_start(str, index);            
            rle += str[index];
            if (len > 1) {
                rle += std::to_string(len); 
            }
            index += len;
        }        
        return rle;
    }

    int count_start(const string& s, int index) {
        char match = s[index];
        auto pos = s.find_first_not_of(match, index);
        int len = (pos == string::npos ? s.size() - index : pos - index);
        return len;
    }

public:
    int getLengthOfOptimalCompression(string s, int k) {
        dp = vector<vector<Info>>(s.size(), vector<Info>(k+1, {0, ' ', 0}));

        // Base cases:
        // for ki == 0
        for (int i=s.size()-1; i>=0; i--) {
            int val = rle_encoded(s.substr(i)).size();
            int count = count_start(s, i);
            dp[i][0] = {val, s[i], count};
        }
        // for index == s.size()-1
        for (int ki=1; ki<=k; ki++) {
            dp[s.size()-1][ki] = {0, ' ', 0};  // this letter is deleted
        }

        // DP case:
        for (int ki=1; ki<=k; ki++) {
            for (int index=s.size()-2; index >=0; index--) {
                Info keep, drop;
                // keep the letter
                {
                    Info rec = dp[index+1][ki];
                    if (s[index] == rec.ch) {
                        // Merge my letter with the RLE of the recursion.
                        keep.ch = s[index];
                        keep.num = rec.num + 1;
                        int delta = std::to_string(keep.num).size() - std::to_string(rec.num).size();
                        if (rec.num == 1) delta++;  // we went from "c" to "c2" as the rle repr
                        keep.val = rec.val + delta;
                    } else {
                        keep.ch = s[index];
                        keep.num = 1;
                        keep.val = rec.val + 1;  // just keep the char without a number after it
                    }
                }
                
                // drop the letter
                {
                    drop = dp[index+1][ki-1];
                }

                if (keep.val < drop.val) {
                    dp[index][ki] = keep;
                } else if (keep.val > drop.val) {
                    dp[index][ki] = drop;
                } else {
                    // fixme: I need to consider both keep and drop
                    // This current else portion is a problem
                    // Sometimes drop is the correct tie breaker, sometimes keep is the correct one
                    // The only real alternative is to put both into a vector and try both
                    // However then upstream, 2 choices lead to 4, lead to 8 etc.
                    // so there is an exponential explosion of the state space
                    // This current line below is causing WA for some choices (and for other choices instead of I flip it to keep)
                    dp[index][ki] = drop;  
                }
            }
        }

        int ans = dp[0][k].val;
        return ans;
    }
};
