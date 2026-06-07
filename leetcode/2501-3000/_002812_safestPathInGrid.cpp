//
// Can use binary search on the solution space
// Consider that we have a shadow-grid, with each cell marked with numbers from [0, n]
// The number says what is the safeness factor of that cell.
//
// Now we can binary search on the solution space between [0, n]
// Say we are testing solution x.
// We write a function asking if it is possible to go from (0, 0) to (n-1, n-1) using only
// cells of safeness x or higher.
// 

#pragma GCC optimize("O3")
#pragma GCC optimize("Ofast,unroll-loops")

struct Coord {
    int r=0;
    int c=0;
    bool operator==(const Coord& rhs) const {
        return (r == rhs.r && c == rhs.c);
    }
    bool operator!=(const Coord& rhs) const {
        return !(*this == rhs);
    }
};

template<>
struct hash<Coord> {
    size_t operator()(const Coord& coord) const {
        return (((size_t)(coord.c) << 16) | (size_t)(coord.r));
    }
};

class Solution {
    int R, C;
    vector<vector<int>> grid;  // stores the original grid
    vector<vector<int>> shadow_grid;  // stores the safeness of each cell.
    int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Static directional array

    // Starts at the thief at loc (r, c) and explores safeness.
    // Use multi-source BFS to avoid timeouts.
    void exploreThief(const vector<Coord>& crds) {
        vector<vector<bool>> visited(R, vector<bool>(C, false));
        deque<std::pair<Coord, int>> mq;
        for (const Coord& crd : crds) {
            mq.push_back(make_pair(Coord(crd.r, crd.c), 0));
            shadow_grid[crd.r][crd.c] = 0;  // this is a thief! not safe.
            visited[crd.r][crd.c] = true;
        }

        while (!mq.empty()) {
            auto elem = mq.front();
            Coord crd = elem.first;
            int depth = elem.second;
            mq.pop_front();
            
            for (auto& d : dirs) {
                Coord neigh = {crd.r+d[0], crd.c+d[1]};
                if (neigh.r < 0 || neigh.r >= R || neigh.c < 0 || neigh.c >= C) {
                    continue;
                }
                if (visited[neigh.r][neigh.c]) continue;
                visited[neigh.r][neigh.c] = true;             
                if (grid[neigh.r][neigh.c] == 1) {
                    // A thief lives here
                    shadow_grid[neigh.r][neigh.c] = 0;
                    continue;
                }
                if (depth + 1 >= shadow_grid[neigh.r][neigh.c]) continue;  // the neigh location is already less safe
                shadow_grid[neigh.r][neigh.c] = depth+1;
                mq.push_back(make_pair(neigh, depth+1));
            }
        }
    }

    // Can we construct a path from {0, 0} to (R-1, C-1) using only cells of safe_val or higher?    
    bool pathExists(int safe_val) {
        vector<Coord> stack;
        vector<vector<bool>> visited(R, vector<bool>(C, false));
        visited[0][0] = true;
        stack.push_back({0, 0});
        if (shadow_grid[0][0] < safe_val) return false;

        while (!stack.empty()) {
            Coord crd = stack.back();
            stack.pop_back();
            if (crd.r == R-1 && crd.c == C-1) {
                return (shadow_grid[crd.r][crd.c] >= safe_val);
            }
                        
            for (auto& d : dirs) {
                Coord neigh = {crd.r+d[0], crd.c+d[1]};
                if (neigh.r < 0 || neigh.r >= R || neigh.c < 0 || neigh.c >= C) {
                    continue;
                }
                if (visited[neigh.r][neigh.c]) continue;
                visited[neigh.r][neigh.c] = true;
                if (shadow_grid[neigh.r][neigh.c] < safe_val) continue;                
                if (neigh.r == R-1 && neigh.c == C-1) {                    
                    return true;                
                }
                stack.push_back(neigh);
            }
        }
        return false;
    }

    int binSearch(int minv, int maxv) {
        if (minv == maxv) return minv;
        if (minv+1 == maxv) {
            if (pathExists(minv)) return minv;
            return maxv;
        }
        int midv = (minv+maxv)/2;
        if (pathExists(midv)) {
            return binSearch(midv, maxv);
        }
        return binSearch(minv, midv);
    }
public:
    int maximumSafenessFactor(vector<vector<int>>& grid_in) {
        grid = grid_in;
        R = grid.size();
        C = grid[0].size();

        // Mark all the safeness values.
        shadow_grid = vector<vector<int>>(R, vector<int>(C, 2*(R+C)));
        vector<Coord> crds;
        for (int r=0; r<R; r++) {
            for (int c=0; c<C; c++) {
                if (grid[r][c] == 1) {
                    crds.push_back({r, c});
                }
            }
        }
        exploreThief(crds);       

        int max_safeness = binSearch(0, R+C);
        return max_safeness;
    }
};
