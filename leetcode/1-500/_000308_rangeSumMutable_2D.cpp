class NumMatrix {
    vector<vector<int>> matrix_;
    // O(r*c) space required.
    vector<vector<int>> cum_sum_;  // cumulative sum of the row
public:
    NumMatrix(vector<vector<int>>& matrix) {
        matrix_ = matrix;
        // Compute the cumulative sum
        cum_sum_ = vector<vector<int>>(matrix.size(), vector<int>(matrix[0].size(), 0));
        for (int r=0; r<matrix_.size(); r++) {            
            for (int c=0; c<matrix_[0].size(); c++) {
                cum_sum_[r][c] = (c == 0 ? 0 : cum_sum_[r][c-1]) + matrix_[r][c];
            }
        }
    }
    
    // O(c) time.
    void update(int row, int col, int val) {
        int delta = val - matrix_[row][col];
        matrix_[row][col] = val;
        for (int c=col; c<matrix_[0].size(); c++) {
            cum_sum_[row][c] += delta;
        }
    }
    
    // O(r) time
    int sumRegion(int row1, int col1, int row2, int col2) {
        int sum=0;
        for (int r=row1; r<=row2; r++) {
            sum += cum_sum_[r][col2] - (col1 == 0 ? 0 : cum_sum_[r][col1-1]);
        }
        return sum;
    }
};

/**
 * Your NumMatrix object will be instantiated and called as such:
 * NumMatrix* obj = new NumMatrix(matrix);
 * obj->update(row,col,val);
 * int param_2 = obj->sumRegion(row1,col1,row2,col2);
 */
