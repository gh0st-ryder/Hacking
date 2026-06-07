struct Info {
    int score=0;  // the score for this student
    int id=0;     // the student_id
};

std::function<bool(const Info&, const Info&)> comp = [](const Info& lhs, const Info& rhs) -> bool {
    if (lhs.score != rhs.score) return lhs.score < rhs.score;   // greater score first
    return lhs.id > rhs.id;  // else lower ID first
};

using MaxHeap = std::priority_queue<Info, vector<Info>, decltype(comp)>;

class Solution {
    unordered_set<string> positive, negative;

    vector<string> splitString(const string& str) {
        vector<string> ans;
        int pos=0;
        while (pos != string::npos && pos < str.size()) {
            auto spc = str.find_first_of(" ", pos);
            string word = str.substr(pos, (spc == string::npos ? string::npos : spc-pos));
            ans.push_back(word);

            // single space between consecutive words per problem statement
            pos = (spc == string::npos ? string::npos : spc+1);
        }
        return ans;
    }
public:
    // O(n + k log n)
    vector<int> topStudents(vector<string>& positive_feedback, vector<string>& negative_feedback, vector<string>& report, vector<int>& student_id, int k) {
        for (const string& word : positive_feedback) {positive.insert(word);}
        for (const string& word : negative_feedback) {negative.insert(word);}

        MaxHeap mh(comp);
        // O(n)
        for (int i=0; i<report.size(); i++) {
            int score = 0;
            vector<string> words = splitString(report[i]);
            for (const string& word : words) {
                if (positive.find(word) != positive.end()) {score += 3;}
                if (negative.find(word) != negative.end()) {score -= 1;}
            }
            mh.push({score, student_id[i]});
        }
        
        vector<int> ans;
        // O(k log n)
        while (ans.size() < k && !mh.empty()) {
            Info info = mh.top();
            mh.pop();
            ans.push_back(info.id);
        }
        return ans;
    }
};
