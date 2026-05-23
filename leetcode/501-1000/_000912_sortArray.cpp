class Solution {
public:
    vector<int> sortArray(vector<int>& nums) {
        priority_queue<int, vector<int>, std::greater<int>> heap;
        for (int n : nums) heap.push(n);
        vector<int> ret;
        while (!heap.empty()) {
            int n = heap.top(); heap.pop();
            ret.push_back(n);
        }
        return ret;
    }
};
