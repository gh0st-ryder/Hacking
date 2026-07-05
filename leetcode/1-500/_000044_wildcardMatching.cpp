#include <algorithm>

class Solution {
    string s, p;
    int getKey(int sind, int pind) {
        return ((sind << 16) | (pind));
    }
    unordered_map<int, bool> cache;

    bool match(int sind, int pind) {        
        if (sind >= s.size() && pind >= p.size()) return true;  // consumed both s and p perfectly        
        if (sind >= s.size() && std::all_of(p.begin()+pind, p.end(), [](char c) -> bool {return c == '*';})) {
            // This is needed for trailing '*' in p when s is already consumed; it is an edge case.
            return true;
        }
        if (sind >= s.size()) return false;  // consumed s, but not p
        if (pind >= p.size()) return false;  // consumed p, but not s

        int key = getKey(sind, pind);
        if (cache.find(key) != cache.end()) return cache[key];

        bool ans = false;
        if (p[pind] == '?') {            
            ans = match(sind+1, pind+1);            
        } else if (p[pind] == '*') {            
            // Match zero or more of s
            for (int si=s.size(); si >= sind; si--) {
                if (match(si, pind+1)) {
                    ans = true;
                    break;
                }
            }            
        } else {            
            // try to match a letter
            ans = (s[sind] == p[pind] && match(sind+1, pind+1));            
        }

        cache[key] = ans;
        return ans;
    }
public:
    bool isMatch(string s_in, string p_in) {
        s = s_in;
        p = p_in;

        // Collapse consecutive '*' in p to simplify and reduce the runtime.
        auto new_end = std::unique(p.begin(), p.end(), [](const char& lhs, const char& rhs) -> bool {
            return (lhs == rhs && lhs == '*');
        });

        p.erase(new_end, p.end());
        return match(0, 0);
    }
};
