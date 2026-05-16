class Solution {
    // Constant space, since this is only ever 10 elements.
    std::unordered_map<char, int> secret_digs, guess_digs;
    void increment(std::unordered_map<char, int>& cont, char ch) {
        if (cont.find(ch) != cont.end()) {
            cont[ch] += 1;
        } else {
            cont[ch] = 1;
        }
    }
    int counts(const std::unordered_map<char, int>& cont, char ch) const {
        if (cont.find(ch) != cont.end()) {
            return cont.find(ch)->second;
        }
        return 0;
    }
public:
    // O(n) time.
    string getHint(string secret, string guess) {
        int bulls=0, cows=0;        
        
        for (int i=0; i<secret.size() && i < guess.size(); i++) {
            if (secret[i] == guess[i]) {
                bulls++;
                secret[i] = guess[i] = '_'; // nullify it
            } else {
                increment(secret_digs, secret[i]);
                increment(guess_digs, guess[i]);
            }
        }

        for (char ch='0'; ch <= '9'; ch++) {
            int s = counts(secret_digs, ch), g = counts(guess_digs, ch);
            cows += std::min(s, g);
        }
 
        string ret = std::to_string(bulls) + "A" + std::to_string(cows) + "B";
        return ret;
    }
};
