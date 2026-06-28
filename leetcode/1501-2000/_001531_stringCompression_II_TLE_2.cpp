class Solution {
    //
    // This solution is functionally correct, but runs into TLE since we are constructing strings over and over
    // I need to modify it so I do not mutate or construct strings.
    // I just need to work with the index and the value k.
    //
    // For DP keep the keys as a pair of (string, num-deletions-leftover)
    //
    // The problem will start with finding
    // optimal(s, k)
    //    
    // we should consider each sub-part of the string as a "chunk" 
    // So (aaabcddd => a3bcd3 => has 4 chunks: a3, b, c, d3)
    // we should go down each chunk and do the following:
    // 
    // for computing optimal(s, k):
    // split s into contiguous sub-sections of same letters
    // iterate through each sub-section and we try to get the answer for:
    //    optimal(s', k') where
    //    s' is obtained by deleting from [0 to min(k, len-of-sub-section)] (call this k_del) from this sub-section
    //    k' = k - k_del

    string construct_key(const string& str, int k) {
        // printf("Input: %s, %d\n", str.c_str(), k);
        string ret = str + "#" + std::to_string(k);
        return ret;
    }

    // The cache for top-down DP.
    unordered_map<string, int> cache;

    // returns the rle encoding for the given string.
    string rle_encoded(const string& str) {
        string rle;
        for (int index=0; index <str.size(); ) {
            char match = str[index];
            auto pos = str.find_first_not_of(match, index);
            int len = (pos == string::npos ? str.size() - index : pos - index);
            rle += match;
            if (len > 1) {
                rle += std::to_string(len); 
            }
            index += len;
        }        
        return rle;
    }

    // s and k are the string and leftover deletions    
    int optimal(const string& s, int k) {
        // printf("Optimal(%s, %d)\n", s.c_str(), k);
        string key = construct_key(s, k);
        if (cache.find(key) != cache.end()) return cache[key];
        if (k == 0) {
            cache[key] = rle_encoded(s).size();
            return cache[key];
        }

        int ans=rle_encoded(s).size();
        int chunks=0;
        for (int index=0; index < s.size();) {
            char match = s[index];
            auto pos = s.find_first_not_of(match, index);
            int len = (pos == string::npos ? s.size() - index : pos - index);           

            string before = s.substr(0, index);
            string after = (pos == string::npos ? "" : s.substr(pos));
            index += len;

            // take out up to a certain number of chars from the above, and recurse.
            int upto = std::min(len, k);
            for (int remove=1; remove <= upto; remove++) {
                int leftover = len - remove;
                string middle(leftover, match);
                int my_opt = optimal(before + middle + after, k - remove);
                ans = std::min(ans, my_opt);
            }            
        }

        cache[key] = ans;
        return ans;
    }

public:
    int getLengthOfOptimalCompression(string s, int k) {
        int remove_at_least_one = optimal(s, k);
        int remove_none = rle_encoded(s).size();
        return std::min(remove_at_least_one, remove_none);
    }
};
