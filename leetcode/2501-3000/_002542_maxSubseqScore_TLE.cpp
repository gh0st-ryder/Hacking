//
// This solution is algorithmically correct, but runs into a TLE
// 19/28 cases pass; rest TLE.
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

        // This holds up to n elements, but at most k elements are popped and repushed per iteration.
        // It holds values from v1, pushed in order as needed.
        std::priority_queue<int, vector<int>, std::less<int>> pq;

        int min_vi2=std::numeric_limits<int>::max();
        long long best = std::numeric_limits<long long>::min();

        for (int i=0; i<vi2.size(); i++) {
            // Consider if the new val from vi2 is min now.
            min_vi2 = std::min(min_vi2, vi2[i].val);

            // Push new number from v1 into our heap.
            pq.push(v1[i]);
            if (pq.size() < k) continue;  // we don't have enough numbers yet.

            // OK, now we will pop k of the biggest numbers from pq (and keep them so we can re-push them later).
            // We will sum these as we pop them.
            // Then we can compute a new candidate as (sum * min_vi2)
            vector<int> repush;
            int ctr=0, sum=0;
            while (ctr < k && !pq.empty()) {
                int val = pq.top();
                pq.pop();
                sum += val;
                repush.push_back(val);
                ctr++;
            }

            long long candidate = (long long)sum * min_vi2;
            best = std::max(best, candidate);

            for (int r : repush) {pq.push(r);}
        }
        return best;
    }
};
