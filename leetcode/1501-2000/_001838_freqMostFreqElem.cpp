class Solution {
    unordered_map<int, int> freqs;
    struct Info {
        int num=0;
        int freq=0;
    };
public:
    int maxFrequency(vector<int>& nums, int k) {        
        for (int n : nums) {freqs[n] += 1;}

        vector<Info> infos;
        for (const auto& kv : freqs) {
            infos.push_back({kv.first, kv.second});
        }
        std::sort(infos.begin(), infos.end(), [](const Info& lhs, const Info& rhs) -> bool {
            if (lhs.num != rhs.num) return lhs.num < rhs.num;
            return lhs.freq < rhs.freq;
        });

        int best_freq = 0;  // will examine for all target numbers
        for (int right=infos.size()-1; right >=0; right--) {
            int remainder = k;  // how many tokens are remaining currently
            int bumps = 0;  // how many numbers were bumped up to equal infos[right].num        
            for (int left = right-1; left >=0; left--) {
                // How many of the left ones can I consume given my current remainder?
                int diff = infos[right].num - infos[left].num;
                int can_consume = std::min(remainder / diff, infos[left].freq);
                bumps += can_consume;
                remainder -= can_consume * diff;
                if (can_consume == 0 || left == 0) {
                    // we are done with right
                    int my_freq = bumps + infos[right].freq;
                    best_freq = std::max(best_freq, my_freq);                    
                    break;  // done with the inner loop
                }
            }
            best_freq = std::max(best_freq, infos[right].freq);
        }
        return best_freq;
    }
};
