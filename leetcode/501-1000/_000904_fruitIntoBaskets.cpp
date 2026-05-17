class Solution {
    // Start with two pointers, both at 0.
    // We will advance the right pointer rightwards, picking fruits as we go.
    // We will also record two things about the fruits
    //    The type of fruit in a set (could use the hash table below)
    //    The index of the rightmost type of that fruit, in a hash table.    
    //
    // when we encounter a new third type of fruit
    // First count the number of fruits of the first two types as (right-left), then adjust best candidate.
    // Now the hash_map will say what "type" of fruit we will dropped
    // This will be the fruit with the leftmost last_index in the map
    // Advance the left pointer to one beyond the last of this type of fruit (using the hash table) 
    // now add the third type of fruit to the hashtable and continue the above sequence
public:
    int totalFruit(vector<int>& fruits) {
        // Key = type of fruit, value = rightmost index of such type between our (left, right)
        unordered_map<int, int> last_index;
        int left=0;  // our sequence of picking starts here
        int right=0; // this is the new fruit we will examine
        int best=0;  // the best sequence encountered so far

        while (right < fruits.size()) {
            // Advance right as far as it will go.
            while (right < fruits.size() && (last_index.size() < 2 || last_index.find(fruits[right]) != last_index.end())) {
                // store the last index
                last_index[fruits[right]] = right;
                right++;
            }

            // Update our best
            int candidate = right - left;
            best = std::max(best, candidate);

            // Advance left until one fruit type is dropped.
            int index=fruits.size();
            int type=0;
            for (const auto& kv : last_index) {
                if (kv.second < index) {
                    type = kv.first;
                    index = kv.second;
                }
            }            
            last_index.erase(fruits[index]);
            left = index+1;            
        }
        return best;
    }
};
