class Solution {

    bool possibleMatch(string del_from, string match) {
        int d_ptr=0, m_ptr=0;
        while (d_ptr < del_from.size() && m_ptr < match.size()) {
            while (d_ptr < del_from.size() && del_from[d_ptr] != match[m_ptr]) {
                d_ptr++;
            }
            if (d_ptr == del_from.size()) return false;
            d_ptr++;
            m_ptr++;
        }
        return m_ptr == match.size();
    }
public:
    string findLongestWord(string s, vector<string>& dictionary) {        
        string best;
        for (const string& match : dictionary) {
            if (possibleMatch(s, match) && best.size() <= match.size()) {                
                if (best.size() < match.size()) {
                    best = match;
                } else {
                    // equal len, store the lexicographically smallest
                    best = (match < best ? match : best);
                }
            }
        }
        return best;
    }
};
