class Solution {
    unordered_map<char, int> last_seen;
    unordered_set<char> seen;
public:
    string smallestSubsequence(string s) {
        for (int i=s.size()-1; i>=0; i--) {
            if (last_seen.find(s[i]) == last_seen.end()) {
                last_seen[s[i]] = i;
            }
        } 

        string mono_stack;
        for (int i=0; i<s.size(); i++) {
            if (mono_stack.empty() || (mono_stack.back() < s[i] && seen.find(s[i]) == seen.end())) {
                mono_stack.push_back(s[i]);
                seen.insert(s[i]);
                // printf("[%d] Adding(1) %c; mono_stack: %s\n", i, s[i], mono_stack.c_str());
                continue;
            }
            // If s[i] is already accounted for, skip it.
            if (seen.find(s[i]) != seen.end()) {
                // printf("[%d] Skipping %c; mono_stack: %s\n", i, s[i], mono_stack.c_str());
                continue;
            }
            // while the top of the mono_stack is greater than curr letter, and there is another instance of it later, remove it from the mono_stack
            while (!mono_stack.empty() && mono_stack.back() > s[i] && last_seen[mono_stack.back()] > i) {
                char rem = mono_stack.back(); mono_stack.pop_back();
                // printf("[%d] Removing %c; mono_stack: %s\n", i, rem, mono_stack.c_str());
                seen.erase(rem);
            }
            // printf("[%d] Adding(2) %c; mono_stack: %s\n", i, s[i], mono_stack.c_str());
            mono_stack.push_back(s[i]);
            seen.insert(s[i]);
        }
        return mono_stack;        
    }
};
