class Solution {
    // WA: 182/187 cases passing
    // The greedy strategy seems to be subtly incorrect!
    // Goddamn greedy problems are the worst.

    vector<int> nums;

    struct Info {
        int num=0;   // the actual number
        int freq=0;  // the freq of the number
        int timestamp=0;  // a token system to invalidate old heap entries
    };

    // A freq table dynamically updates as we build chains.
    // Key: number; Value: dynamic freq of the number (reduced when consumed)
    unordered_map<int, int> freq;

    // A per number timestamp to invalidate old entries in the heap.
    // Key: number; Value: timestamp (monotonically increasing, higher is newer)
    unordered_map<int, int> timestamp;

    // A comparator function used for the heap.
    std::function<bool(const Info&, const Info&)> comp = [](const Info& lhs, const Info& rhs) -> bool {
        if (lhs.freq != rhs.freq) return lhs.freq < rhs.freq;
        if (lhs.num != rhs.num) return lhs.num > rhs.num;
        return lhs.timestamp < rhs.timestamp;
    };
    using Heap = priority_queue<Info, vector<Info>, decltype(comp)>;
    Heap heap;

    void init() {
        heap = Heap(comp);  // Instantiate the heap properly
        freq.clear();
        timestamp.clear();
        for (int i=0; i<nums.size(); i++) {
            freq[nums[i]]++;
            timestamp[nums[i]] = 0;
        }
        for (const auto& kv : freq) {
            heap.push({kv.first, kv.second, 0});
        }
    }

    // Builds a chain greedily starting from the given number, consuming entries from the heap.
    // If it can't build at least a 3 length chain, returns false.
    // Else
    // Builds the longest chain it can
    // Updates the heap with num in the chain, with new freq (if non-zero), and new timestamps
    // Updates the freq and timestamps hash tables too
    // returns true.
    bool buildChain(int num) {
        unordered_map<int, int> updated_freq;
        unordered_map<int, int> updated_timestamp;
        
        while (1) {
            if (freq.find(num) == freq.end() || freq[num] == 0) break;
            updated_freq[num] = freq[num] - 1;
            updated_timestamp[num] = timestamp[num] + 1;
            num = num+1;  // try to get the next if it exists
        }
        int chain_len = updated_freq.size();
        if (chain_len < 3) {
            // printf("Chain_len < 3 for these numbers: \n");
            for (const auto& kv : updated_freq) {
                // printf("%d, ", kv.first);
            }
            // printf("\n");
            return false;
        }

        // Now update all the bookkeeping
        for (const auto& kv : updated_freq) {
            freq[kv.first] = updated_freq[kv.first];
            timestamp[kv.first] = updated_timestamp[kv.first];
            Info info = {kv.first, freq[kv.first], timestamp[kv.first]};
            if (freq[kv.first] > 0) {
                heap.push(info);
            }
        }
        return true;
    }

public:
    // The basic logic is as follows
    // We first count the occurrences of each number
    // We start with the number with the largest occurrence, and try to build the longest chain increasing by 1 each time
    // If this is <3 immediately return false
    // else we update the frequencies to mark off the freq of the numbers consumed in the chain (decrement each by 1)
    // And we keep going with the algorithm
    // if in the end we consume all numbers, then we succeeded, else failed.
    //
    // Note that timestamp is used to mark off old invalid entries in the heap, when we add new entries for the same number
    // This is because our heap does not have a delete API to remove entries
    bool isPossible(vector<int>& nums_in) {
        nums = nums_in;
        init();

        while (!heap.empty()) {
            Info info = heap.top(); 
            heap.pop();
            if (info.timestamp < timestamp[info.num]) {
                continue;  // processed a stale entry
            }

            // Found a number I need to build a longest possible chain from
            if (!buildChain(info.num)) return false;  // could not build chain given current bookkeeping.
        }
        // Were all numbers consumed?
        bool consumed = true;
        for (const auto& kv : freq) {
            if (kv.second != 0) {
                // printf("Unconsumed (num -> freq) %d -> %d\n", kv.first, kv.second);
                consumed = false;
            }
        }
        return consumed;
    }
};
