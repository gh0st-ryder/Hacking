class Solution {
public:
    vector<vector<int>> groupThePeople(vector<int>& groupSizes) {
        unordered_map<int, vector<int>> groups;
        vector<vector<int>> ans;
        for (int i=0; i<groupSizes.size(); i++) {
            groups[groupSizes[i]].push_back(i);
            if (groups[groupSizes[i]].size() == groupSizes[i]) {
                ans.push_back(groups[groupSizes[i]]);
                groups[groupSizes[i]].clear();
            }
        }
        return ans;
    }
};
