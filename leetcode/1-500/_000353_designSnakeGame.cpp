struct Coord {
    int r=0;
    int c=0;    

     bool operator==(const Coord& rhs) const {
        return (r == rhs.r && c == rhs.c);
    }
};

template<>
struct hash<Coord> {
    size_t operator()(const Coord& coord) const {
        return (size_t)((unsigned int)(coord.r) | ((unsigned int)(coord.c) << 16));
    }
};

class SnakeGame {
    // Snake can occupy many cells.
    enum Cell {EMPTY, FOOD, SNAKE};

    // debug helper
    void printSnake() const {
        printf("Snake: ");
        for (const Coord& c : snake_) {
            Cell cell_e = board_[c.r][c.c];
            string cell = (cell_e == EMPTY ? "E" : (cell_e == FOOD ? "F" : "S"));
            printf("[%d, %d] %s, ", c.r, c.c, cell.c_str());
        }
        printf("\n");
    }

    // Can return negative coord or out of bounds coords or coords on snake.
    // Should run it on the head of the snake.
    Coord nextCoord(const Coord& curr, string dir) const {
        if (dir == "R") {
            return Coord{curr.r, curr.c+1};
        } else if (dir == "L") {
            return Coord{curr.r, curr.c-1};
        } else if (dir == "D") {
            return Coord{curr.r+1, curr.c};
        } else { // dir == "U"
            return Coord{curr.r-1, curr.c};
        }
    }

    bool isOutOfBounds(const Coord& coord) const {
        bool ret = (coord.r < 0 || coord.r >= h_ || coord.c < 0 || coord.c >= w_);
        // if (ret) { printf("Out of bounds at coord(%d, %d)\n", coord.r, coord.c);  }
        return ret;
    }
    
    // The head in curr time tick, can move into the tail of the prev time tick!
    // So filter out the last tail position of the snake.
    // Called with coord = new location of head
    bool eatsSnake(const Coord& coord) const {
        if (coord == snake_.back()) return false;
        bool ret = board_[coord.r][coord.c] == SNAKE;
        // if (ret) printf("Eats snake at coord(%d, %d)\n", coord.r, coord.c);
        return ret;
    }

    // The food piece turns into the snake's new head location. Size of snake thus grew by 1.
    void captureFood(const Coord& coord) {
        score_++;
        board_[coord.r][coord.c] = SNAKE;
        snake_.push_front(coord);
        // printf("Ate food at [%d, %d]\n", coord.r, coord.c);

        // Place next food if not done
        if (next_food_idx_ != -1) {
            Coord f = {food_[next_food_idx_][0], food_[next_food_idx_][1]};
            board_[f.r][f.c] = FOOD;
            next_food_idx_++;
            if (next_food_idx_ == food_.size()) {
                next_food_idx_ = -1;
            }
        }
    }
    
    void moveSnake(string dir) {
        Coord newloc = nextCoord(snake_.front(), dir);
        if (isOutOfBounds(newloc) || eatsSnake(newloc)) {
            game_over_ = true;
            return;
        }
        if (board_[newloc.r][newloc.c] == FOOD) {
            captureFood(newloc);
            return;
        }
        // Normal movement of the snake.
        // printf("Setting [%d, %d] to snake\n", newloc.r, newloc.c);
        board_[newloc.r][newloc.c] = SNAKE;
        snake_.push_front(newloc);
        Coord erase = snake_.back();
        if (newloc != erase) {
            board_[erase.r][erase.c] = EMPTY;
            // printf("Setting [%d, %d] to empty\n", erase.r, erase.c);
        }
        
        snake_.pop_back();
    }

    // Tracks a snake's position from beginning (head) to end (tail).
    // This is an "ordered" list from head to tail coords.
    deque<Coord> snake_;
    unordered_set<Coord> snake_set_; // Also track the snake in a hashtable for collision detection.

    // The main board
    vector<vector<Cell>> board_;
    int h_, w_;
    vector<vector<int>> food_;
    int next_food_idx_;
    int score_=0;
    bool game_over_=false;

public:
    SnakeGame(int width, int height, vector<vector<int>>& food) {
        h_ = height;
        w_ = width;
        board_ = vector<vector<Cell>>(height, vector<Cell>(width, EMPTY));
        food_ = food;
        if (!food.empty()) {
            board_[food[0][0]][food[0][1]] = FOOD;
            next_food_idx_ = (food.size() == 1 ? -1 : 1);
        } else {
            next_food_idx_ = -1;
        }
        board_[0][0] = SNAKE;
        snake_.push_back(Coord{0, 0});
    }
    
    int move(string direction) {
        moveSnake(direction);
        // printSnake();
        if (game_over_) return -1;
        return score_;
    }
};

/**
 * Your SnakeGame object will be instantiated and called as such:
 * SnakeGame* obj = new SnakeGame(width, height, food);
 * int param_1 = obj->move(direction);
 */
