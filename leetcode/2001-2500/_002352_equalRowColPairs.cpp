class Solution {
    // Key: hash of the entire row (or column)
    // Value: the indices of the rows (or columns) that make up that hash.
    unordered_map<string, vector<int>> row_hashes, col_hashes;

    string getRowHash(const vector<vector<int>>& grid, int row) {
        string hash;
        for (int c=0; c<grid[0].size(); c++) {
            hash += std::to_string(grid[row][c]) + "#";
        }
        return hash;
    }
    string getColHash(const vector<vector<int>>& grid, int col) {
        string hash;
        for (int r=0; r<grid.size(); r++) {
            hash += std::to_string(grid[r][col]) + "#";
        }
        return hash;
    }
public:
    // O(n^2) algorithm.
    int equalPairs(vector<vector<int>>& grid) {
        for (int r=0; r<grid.size(); r++) {
            string hash = getRowHash(grid, r);
            row_hashes[hash].push_back(r);
        }
        for (int c=0; c<grid[0].size(); c++) {
            string hash = getColHash(grid, c);
            col_hashes[hash].push_back(c);
        }
        int total=0;
        for (const auto& kv : row_hashes) {
            if (col_hashes.find(kv.first) == col_hashes.end()) continue;
            total += kv.second.size() * col_hashes[kv.first].size();
        }
        return total;
    }
};
