struct Cell {
    int r=0;
    int c=0;
    bool operator==(const Cell& rhs) const {
        return (r == rhs.r && c == rhs.c);
    }
};

template<>
struct hash<Cell> {
    size_t operator()(const Cell& cell) const {
        return ((cell.r) | (cell.c << 16));
    }
};

class Solution {
    enum State {NOT_SET=0, LOCKED, NOT_LOCKED};
    // states[r][c] is locked if that cell is an island cell that cannot walk off the grid.
    // states[r][c] is not_locked if that cell is an island cell that can walk off the grid.
    vector<vector<State>> states;
    vector<vector<int>> grid;
    int rows=0;
    int cols=0;

    bool isOutOfBounds(Cell cell) {
        return (cell.r < 0 || cell.r >= rows || cell.c < 0 || cell.c >= cols);
    }

    // User beware: This function may return out of bounds cells by design!
    vector<Cell> getNeighbors(Cell cell) {
        vector<Cell> ret;        
        ret.push_back({cell.r-1, cell.c});
        ret.push_back({cell.r+1, cell.c});
        ret.push_back({cell.r, cell.c-1});
        ret.push_back({cell.r, cell.c+1});
        return ret;
    }

    // Returns true if the island group connected to cell is water-locked.
    bool isGroupLocked(Cell cell_in) {
        return bfs(cell_in, false, NOT_SET);
    }

    void setIslandValue(Cell cell_in, State lock_val) {
        bfs(cell_in, true, lock_val);
    }

    // Performs a bfs. 
    // If set_state is false, then it simply detects if the island is water-locked and returns that (may leave early).
    // If set_state is true, it walks the island and sets ALL those cells to lock_val.
    bool bfs(Cell cell_in, bool set_state, State lock_val) {
        unordered_set<Cell> visited;
        deque<Cell> myq;
        myq.push_back(cell_in);
        visited.insert(cell_in);
        if (set_state) {
            states[cell_in.r][cell_in.c] = lock_val;
        }

        while (!myq.empty()) {
            Cell cell = myq.front();
            myq.pop_front();

            for (Cell neigh : getNeighbors(cell)) {
                if (visited.find(neigh) != visited.end()) continue;
                if (isOutOfBounds(neigh)) {
                    if (set_state == false) return false;  // out of bounds walk, this island is NOT water-locked
                    continue;  // skip out of bounds cell
                }
                if (grid[neigh.r][neigh.c] == 0) continue;  // skip water cells adjacent to me
                if (set_state) {
                    states[neigh.r][neigh.c] = lock_val;
                }
                visited.insert(neigh);
                myq.push_back(neigh);
            }
        }
        return true;  // didn't early return, so this island is water-locked
    }
public:
    int numEnclaves(vector<vector<int>>& grid_in) {
        grid = grid_in;        
        rows = grid.size();
        cols = grid[0].size();
        states = vector<vector<State>>(rows, vector<State>(cols, NOT_SET));

        for (int r=0; r<rows; r++) {
            for (int c=0; c<cols; c++) {
                if (grid[r][c] == 0) continue;  // water cell; skip
                if (states[r][c] != NOT_SET) continue;  // skip processed islands
                if (isGroupLocked({r, c})) {
                    setIslandValue({r, c}, LOCKED);
                } else {
                    setIslandValue({r, c}, NOT_LOCKED);
                }
            }
        }

        int count=0;
        for (int r=0; r<rows; r++) {
            for (int c=0; c<cols; c++) {
                if (states[r][c] == LOCKED) count++;
            }
        }
        return count;
    }
};
