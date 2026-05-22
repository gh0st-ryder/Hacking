struct Cell {
    int r=0;
    int c=0;
    int level=0;

    bool operator==(const Cell& rhs) const {
        return (r == rhs.r && c == rhs.c && level == rhs.level);
    }
};

template<>
struct hash<Cell> {
    size_t operator()(const Cell& cell) const {
        return (size_t)((cell.r) | (cell.c << 16) | ((size_t)(cell.level) << 32));
    }
};

class Solution {
    // The grid and it's dimensions.
    vector<vector<int>> grid;
    int rows=0;
    int cols=0;

    vector<Cell> getNeighbors(const Cell& cell) {
        vector<Cell> ret;
        if (cell.r > 0) {
            ret.push_back({cell.r-1, cell.c, 0});
        }
        if (cell.r < rows-1) {
            ret.push_back({cell.r+1, cell.c, 0});
        }
        if (cell.c > 0) {
            ret.push_back({cell.r, cell.c-1, 0});
        }
        if (cell.c < cols-1) {
            ret.push_back({cell.r, cell.c+1, 0});
        }
        return ret;
    }

    // Visits each cell in the grid until it finds a 1.
    // Then it changes the island connected to that 1 into 2's to differentiate between the two islands.
    // Returns all the cells in the 2's island, with a 0 as the level value.
    deque<Cell> markIsland2() {
        Cell cell;
        bool done = false;
        for (int r=0; r<rows; r++) {
            for (int c=0; c<cols; c++) {
                if (grid[r][c] == 1) {
                    cell.r = r;
                    cell.c = c;
                    done = true;
                    break;
                }                
            }
            if (done) break;
        }
        deque<Cell> island;        
        deque<Cell> myq;
        myq.push_back(cell);
        island.push_back(cell);

        unordered_set<Cell> visited;
        visited.insert(cell);

        grid[cell.r][cell.c] = 2;        

        while (!myq.empty()) {
            Cell cell = myq.front(); 
            myq.pop_front();
            
            for (const Cell& neigh : getNeighbors(cell)) {
                if (visited.find(neigh) != visited.end()) continue;
                if (grid[neigh.r][neigh.c] != 1) continue;
                grid[neigh.r][neigh.c] = 2;
                visited.insert(neigh);  // so you only examine it one time, and multiple cells don't push this onto the deque
                myq.push_back(neigh);
                island.push_back(neigh);
            }
        }

        return island;
    }
public:
    int shortestBridge(vector<vector<int>>& grid_in) {
        grid = grid_in;
        rows = grid.size();
        cols = grid[0].size();

        deque<Cell> myq = markIsland2();
        unordered_set<Cell> visited;

        while (!myq.empty()) {
            Cell cell = myq.front();
            myq.pop_front();

            for (const Cell& neigh : getNeighbors(cell)) {
                if (visited.find(neigh) != visited.end()) continue;
                if (grid[neigh.r][neigh.c] == 1) {
                    // Found the shortest length
                    return cell.level;
                }
                if (grid[neigh.r][neigh.c] != 0) continue;
                
                myq.push_back({neigh.r, neigh.c, cell.level+1});
                visited.insert({neigh.r, neigh.c, cell.level+1});
            }

        }

        // Should not get here... it means there was no second island.
        return -1;
    }
};
