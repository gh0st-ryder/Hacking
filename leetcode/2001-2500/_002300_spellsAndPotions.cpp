class Solution {
    struct Info {
        int val=0;    // the value of this spell
        int index=0;  // the index of this value in the original spells array
    };
public:
    // O(nlogn + mlogm) algorithm.
    vector<int> successfulPairs(vector<int>& spells, vector<int>& potions, long long success) {
        vector<Info> sinfos;
        for (int i=0; i<spells.size(); i++) {sinfos.push_back({spells[i], i});}
        std::sort(sinfos.begin(), sinfos.end(), [](const Info& lhs, const Info& rhs) -> bool {
            if (lhs.val != rhs.val) return lhs.val < rhs.val;
            return lhs.index < rhs.index;
        });
        std::sort(potions.begin(), potions.end());

        // Key: the index in the original array, Value: the number of successful pairings.
        unordered_map<int, int> pairs;
        int right=potions.size()-1;  // the next potion to examine (moves from right to left in decreasing order of potion strength).
        for (const Info& info : sinfos) {
            while (right >= 0 && (long long)(info.val) * potions[right] >= success) {
                right--;
            }
            int count = potions.size() - 1 - right;
            pairs[info.index] = count;
        }

        vector<int> ans(spells.size(), 0);
        for (const auto& kv : pairs) {
            ans[kv.first] = kv.second;
        }
        return ans;
    }
};
