struct Coord {
    int x=0;
    int y=0;
    bool operator==(const Coord& rhs) const {
        return (x == rhs.x && y == rhs.y);
    }
    bool operator!=(const Coord& rhs) const {
        return (!(*this == rhs));
    }
    bool operator<(const Coord& rhs) const {
        if (x != rhs.x) return x < rhs.x;
        return y < rhs.y;
    }
};

template<>
struct hash<Coord> {
    size_t operator()(const Coord& crd) const {
        return ((size_t)(crd.x) | (size_t)(crd.y << 16));
    }
};

struct Info {
    Coord c1;
    Coord c2;
    int distance=0;
};

std::function<bool(const Info&, const Info&)> comp = [](const Info& lhs, const Info& rhs) -> bool {
    if (lhs.distance != rhs.distance) return lhs.distance > rhs.distance;  // lower distance first
    if (lhs.c1 != rhs.c1) return lhs.c1 < rhs.c1;  // arbitrary tiebreaker
    return lhs.c2 < rhs.c2;  // arbitrary tiebreaker
};

using MinHeap = priority_queue<Info, vector<Info>, decltype(comp)>;

class Solution {
    // Union find data.
    vector<int> leaders;
    vector<int> sizes;

    unordered_map<Coord, int> crd_to_node;

    void uf_init(const vector<vector<int>>& points) {
        int num_points = points.size();
        for (int i=0; i<num_points; i++) {
            crd_to_node[{points[i][0], points[i][1]}] = i;
            leaders.push_back(i);
            sizes.push_back(1);
        }
    }

    // Finds the leader for this group.
    int uf_find(Coord crd) {
        int node = crd_to_node[crd];
        while (leaders[node] != node) {
            node = leaders[node];
        }
        return node;
    }

    // Unites two groups.
    void uf_union(Coord c1, Coord c2) {
        int l1 = uf_find(c1);
        int l2 = uf_find(c2);
        if (l1 == l2) return;  // already union'd
        if (sizes[l1] >= sizes[l2]) {
            leaders[l2] = l1;
            sizes[l1] += sizes[l2];
        } else {
            leaders[l1] = l2;
            sizes[l2] += sizes[l1];
        }
    }

    MinHeap mh;
public:
    int minCostConnectPoints(vector<vector<int>>& points) {        
        uf_init(points);
        mh = MinHeap(comp);
        for (int i=0; i<points.size(); i++) {
            for (int j=i+1; j<points.size(); j++) {
                Coord ci = {points[i][0], points[i][1]};
                Coord cj = {points[j][0], points[j][1]};
                int distance = std::abs(points[i][0] - points[j][0]) + std::abs(points[i][1] - points[j][1]);
                mh.push({ci, cj, distance});
            }
        }

        // Kruskal's algorithm to construct the minimum spanning tree.
        int total=0;
        while (!mh.empty()) {
            Info info = mh.top();
            mh.pop();
            int l1 = uf_find(info.c1);
            int l2 = uf_find(info.c2);
            if (l1 == l2) continue;
            total += info.distance;
            uf_union(info.c1, info.c2);
        }
        return total;
    }
};
