//
// Clears all test cases!
//
class Solution {
    struct ValIndex {
        int val=0;
        int index=0;
    };
public:
    long long maxScore(vector<int>& nums1, vector<int>& nums2, int k) {
        vector<ValIndex> vi2;
        for (int i=0; i<nums2.size(); i++) {
            vi2.push_back({nums2[i], i});
        }
        std::sort(vi2.begin(), vi2.end(), [&nums1](const ValIndex& lhs, const ValIndex& rhs) -> bool {
            if (lhs.val != rhs.val) return lhs.val > rhs.val;  // greater val first
            if (nums1[lhs.index] != nums1[rhs.index]) return nums1[lhs.index] > nums1[rhs.index];  // for same nums2 val, pick the index with the higher nums1 val
            return lhs.index < rhs.index;  // arbitrary tiebreaker here.
        });

        // v1 is nums1 sorted in the same order of indices as vi2.
        vector<int> v1;
        for (const auto& vi : vi2) {
            v1.push_back(nums1[vi.index]);
        }

        // Now we will work with v1 and vi2.

        // Instead of constantly popping and repushing, we only keep the k largest elements in a MIN heap.
        // We also keep a running sum of ALL elements from v1 encountered so far
        // when the heap becomes size k+1, we pop exactly one, and delete that from the running sum
        // So the running sum always contains the largest k elements encountered so far from v1!
        std::priority_queue<int, vector<int>, std::greater<int>> pq;

        int min_vi2=std::numeric_limits<int>::max();
        long long running_sum = 0;
        long long best = std::numeric_limits<long long>::min();

        for (int i=0; i<vi2.size(); i++) {            
            // Consider if the new val from vi2 is min now.
            min_vi2 = std::min(min_vi2, vi2[i].val);

            // Push new number from v1 into our heap.
            pq.push(v1[i]);
            running_sum += v1[i];
            if (pq.size() < k) continue;  // we don't have enough numbers yet.
            if (pq.size() == k) {
                long long candidate = running_sum * min_vi2;
                best = std::max(best, candidate);   
                continue;
            }

            // OK, now we will pop exactly 1 number
            // remove that from the running sum.
            // Then we can compute a new candidate as (running_sum * min_vi2)
            int got = pq.top();
            pq.pop();
            running_sum -= got;

            long long candidate = running_sum * min_vi2;
            best = std::max(best, candidate);            
        }
        return best;
    }
};
