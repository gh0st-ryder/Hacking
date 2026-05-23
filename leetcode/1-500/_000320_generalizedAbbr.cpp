
class Solution {
    // [start_index][size] = {all strings, including a lone number string}
    vector<vector<vector<string>>> table;
    const string digits = "0123456789";

public:
    vector<string> generateAbbreviations(string word) {
        // Init the table.
        table = vector<vector<vector<string>>>(word.size(), vector<vector<string>>(word.size()+1, vector<string>()));
        
        // Base case, sz = 1 strings
        for (int i=0; i<word.size(); i++) {
            table[i][1].push_back(string(1, word[i]));
            table[i][1].push_back(std::to_string(1));
        }        

        // DP case, increasing sizes, for all starting indices
        // Combine a single letter choice with sz sized choices
        for (int sz = 1; sz<word.size(); sz++) {
            for (int index=0; index < word.size(); index++) {
                if (index + sz >= word.size()) continue;                
                vector<string> new_choices;
                for (const auto& s1 : table[index][1]) {                    
                    for (const auto& s2 : table[index+1][sz]) {                        
                        if (std::isdigit(s1[0]) && std::isdigit(s2[0])) {
                            int pos = s2.find_first_not_of(digits);
                            int s2num = (pos == string::npos ? std::stoi(s2) : std::stoi(s2.substr(0, pos)));
                            string s2rem = (pos == string::npos ? "" : s2.substr(pos));
                            int new_num = 1 + s2num;
                            string str = std::to_string(new_num) + s2rem;                            
                            new_choices.push_back(str);
                        } else {
                            string str = s1 + s2;                            
                            new_choices.push_back(str);
                        }
                    }
                }
                table[index][sz+1] = new_choices;
            }
        }

        vector<string> ret;
        for (const string& str : table[0][word.size()]) {            
            ret.push_back(str);
        }
        return ret;
    }
};
