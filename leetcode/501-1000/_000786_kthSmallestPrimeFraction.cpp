class Solution {
    vector<int> nums;
    struct Info {
        int num_index=0;
        int den_index=0;
    };

    // My heap will maintain up to 1 (or 0) Info entity for each unique numerator
    // When my heap pops that quantity, I will insert another quantity for that numerator, if available.

    std::function<bool(const Info&, const Info&)> comp = [this](const Info& lhs, const Info& rhs) -> bool {
        double lhs_d = (double)(this->nums[lhs.num_index]) / this->nums[lhs.den_index];
        double rhs_d = (double)(this->nums[rhs.num_index]) / this->nums[rhs.den_index];
        if (lhs_d != rhs_d) return lhs_d > rhs_d;  // smaller fraction goes to the top of the heap.
        if (lhs.num_index != rhs.num_index) return lhs.num_index > rhs.num_index;
        return lhs.den_index > rhs.den_index;
    };
    using Heap=std::priority_queue<Info, vector<Info>, decltype(comp)>;
    Heap heap;
public:
    vector<int> kthSmallestPrimeFraction(vector<int>& arr, int k) {
        heap = Heap(comp);  // instantiate the heap.
        nums = arr;

        // Populate the heap
        for (int i=0; i<nums.size() - 1; i++) {
            heap.push({i, (int)(nums.size())-1});
        }

        int count=0;
        while (!heap.empty()) {
            Info info = heap.top();
            heap.pop();
            count++;
            if (count == k) {return {nums[info.num_index], nums[info.den_index]};}
            if (info.num_index + 1 == info.den_index) {continue;}  // done with this numerator
            heap.push({info.num_index, info.den_index-1});
        }
        return {};  // should not get here
    }
};
