class Solution {
public:
    vector<int> minOperations(string boxes) {
        vector<int> prefix_moves(boxes.size(), 0);
        vector<int> suffix_moves(boxes.size(), 0);

        int count=0;
        for (int i=0; i<boxes.size(); i++) {
            prefix_moves[i] = (i == 0 ? 0 : prefix_moves[i-1] + count);
            count += (boxes[i] == '1' ? 1 : 0);
        }
        count=0;
        for (int i=boxes.size()-1; i>=0; i--) {
            suffix_moves[i] = (i == boxes.size()-1 ? 0 : suffix_moves[i+1] + count);
            count += (boxes[i] == '1' ? 1 : 0);
        }
        vector<int> ans;
        for (int i=0; i<boxes.size();i++) {
            int total = prefix_moves[i] + suffix_moves[i];
            ans.push_back(total);
        }
        return ans;
    }
};
