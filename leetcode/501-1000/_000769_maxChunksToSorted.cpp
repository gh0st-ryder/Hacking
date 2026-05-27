
class Solution {
public:
    int maxChunksToSorted(vector<int>& arr) {
        int chunks=0;
        int max_so_far=-1;
        for (int i=0; i<arr.size(); i++) {
            if (arr[i] > max_so_far) {
                max_so_far = arr[i];
            }
            if (i == max_so_far) {
                chunks++;
            }
        }
        return chunks;
    }
};
