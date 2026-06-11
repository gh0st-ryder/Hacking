class Solution {
    // 
    // This problem has a description from hell... the fucking representation doesn't make sense.
    // This problem's representation in moveCost is some grade-A+ level bullshit.
    //

    int R, C;
    struct Coord {
        int r=0; 
        int c=0;
    };

    // I need to map a grid value to its coordinates
    unordered_map<int, Coord> tag_to_coord;

    // index outer: row
    // index mid: start col in row
    // index inner: end col in row+l
    vector<vector<vector<int>>> moveCostRepr;

    // The min cost to go from cell [r, c] to the bottomMost layer
    vector<vector<int>> minCost;

    // Recurrence relation:
    //
    // minCost(r, c) = grid(r, c) + min over ci going from [0, n-1] {minCost(r+1, ci)) + moveCostRepr(r, c, ci)}
public:
    int minPathCost(vector<vector<int>>& grid, vector<vector<int>>& moveCost) {
        R = grid.size();
        C = grid[0].size();        
        moveCostRepr = vector<vector<vector<int>>>(R, vector<vector<int>>(C, vector<int>(C, 0)));
        minCost = vector<vector<int>>(R, vector<int>(C, 0));
        for (int r=0; r<R; r++) {
            for (int c=0; c<C; c++) {
                tag_to_coord[grid[r][c]] = {r, c};
            }
        }
        for (int tag=0; tag < R*C; tag++) {
            Coord crd = tag_to_coord[tag];
            for (int ci=0; ci<C; ci++) {
                moveCostRepr[crd.r][crd.c][ci] = moveCost[tag][ci];
            }
        }

        // DP base case.
        for (int c=0; c<C; c++) {
            minCost[R-1][c] = grid[R-1][c];
        }

        // DP cases
        for (int r=R-2; r>=0; r--) {
            for (int c=0; c<C; c++) {
                minCost[r][c] = std::numeric_limits<int>::max();
                for (int ci=0; ci<C; ci++) {
                    minCost[r][c] = std::min(minCost[r][c], minCost[r+1][ci] + moveCostRepr[r][c][ci]);
                }
                minCost[r][c] += grid[r][c];
            }
        }

        int best = minCost[0][0];
        for (int c=1; c<C; c++) {
            best = std::min(best, minCost[0][c]);
        }
        return best;
    }
};
