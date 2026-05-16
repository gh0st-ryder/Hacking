struct Coord {
    int x=0;
    int y=0;

    bool operator==(const Coord& rhs) const {
        return x == rhs.x && y == rhs.y;
    }
};

template<>
struct hash<Coord> {
    size_t operator()(const Coord& crd) const {
        return ((crd.x) | (crd.y << 16));
    }
};

class Solution {
    vector<Coord> coords;
    double reflected_x;  // when found, the line x = reflected_x is the line parallel to the y-axis that we need

    bool findCandidate() {
        int y_check = coords[0].y;
        vector<Coord> bottoms;
        for (const Coord& crd : coords) {
            if (crd.y == y_check) bottoms.push_back(crd);
        }
        // Now sort on the x coord.
        std::sort(bottoms.begin(), bottoms.end(), [](const Coord& lhs, const Coord& rhs) -> bool {
            if (lhs.x != rhs.x) return lhs.x < rhs.x;
            return lhs.y < rhs.y;
        });
        
        if (bottoms.size() == 1) {
            reflected_x = bottoms[0].x;
            return true;
        }
        reflected_x = (bottoms.back().x - bottoms[0].x) / 2.0 + bottoms[0].x;
        return true;
    }
public:
    bool isReflected(vector<vector<int>>& points) {
        if (points.size() == 0) return true;

        for (const vector<int>& point: points) {
            coords.push_back({point[0], point[1]});
        }
        std::sort(coords.begin(), coords.end(), [](const Coord& lhs, const Coord& rhs) -> bool {
            if (lhs.y != rhs.y) return lhs.y < rhs.y;
            return lhs.x < rhs.x;
        });

        if (!findCandidate()) {
            return false;  //  nothing found
        }

        // Found a candidate. Check to make sure all points are reflected along this candidate.
        unordered_map<Coord, bool> matched;
        for (const Coord& crd : coords) {
            matched[crd] = false;
        }
        // Now let's reflect each point along reflected_x and mark them off
        for (const Coord& crd : coords) {
            int new_x = reflected_x - crd.x + reflected_x;
            Coord check = {new_x, crd.y};
            if (matched.find(check) != matched.end()) {
                matched[check] = true;
                matched[crd] = true;
            } else {
                return false;
            }
        }
        return true;
    }
};
