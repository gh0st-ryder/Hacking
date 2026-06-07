class Solution {
public:
    int longestPalindrome(vector<string>& words) {
        unordered_map<string, int> htable;
        for (const string& w : words) {
            htable[w] += 1;
        }
        vector<string> fronts;
        vector<string> backs;
        for (const auto& kv : htable) {            
            string me = kv.first;
            string rev = me;
            std::reverse(rev.begin(), rev.end());            
            if (me[0] == me[1]) {
                // same letters in the string
                int take = kv.second/2;
                for (int i=0; i < take; i++) {
                    fronts.push_back(me);
                    htable[me]--;
                    backs.push_back(me);
                    htable[me]--;
                }
            } else if (htable.find(rev) != htable.end() && htable[rev] > 0) {
                // matching reverse string
                int take = std::min(kv.second, htable[rev]);  // match all the occurrences we can match
                for (int i=0; i < take; i++) {
                    fronts.push_back(me);
                    htable[me]--;
                    backs.push_back(rev);
                    htable[rev]--;
                }
            }
        }

        // now for the singles, take exactly one
        string special;
        for (const auto& kv : htable) {
            string me = kv.first;
            if (me[0] == me[1] && kv.second >= 1) {
                special = me;
                break;
            }
        }

        string ans;
        for (int i=0; i<fronts.size(); i++) {
            ans += fronts[i];
        }
        ans += special;
        for (int i=backs.size()-1; i>=0; i--) {
            ans += backs[i];
        }
        return ans.size();
    }
};
