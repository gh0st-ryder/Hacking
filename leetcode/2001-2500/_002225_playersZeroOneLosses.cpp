class Solution {
    unordered_map<int, int> win_freq, loss_freq;
public:
    vector<vector<int>> findWinners(vector<vector<int>>& matches) {
        for (const auto& match : matches) {
            win_freq[match[0]] += 1;
            loss_freq[match[1]] += 1;
        }
        vector<vector<int>> ans;
        vector<int> winners, loseOnes;
        for (const auto& kv : win_freq) {
            if (loss_freq.find(kv.first) == loss_freq.end()) {
                winners.push_back(kv.first);
            }
        }
        for (const auto& kv : loss_freq) {
            if (kv.second == 1) {
                loseOnes.push_back(kv.first);
            }
        }
        std::sort(winners.begin(), winners.end());
        std::sort(loseOnes.begin(), loseOnes.end());
        ans.push_back(winners);
        ans.push_back(loseOnes);
        return ans;
    }
};
