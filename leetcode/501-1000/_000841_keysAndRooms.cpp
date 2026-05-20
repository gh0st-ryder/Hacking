class Solution {
    vector<vector<int>> rooms;

    // Contains the set of room keys that I currently have
    // We may have multiple copies of a room key.
    vector<int> room_keys;

    // Contains which rooms I have visited so far
    unordered_set<int> visited;

    // Performs a dfs, collecting keys along the way
    // Honors keyset while trying to enter a new room.
    // room is the room I'm trying to enter
    bool dfs(int room) {
        if (visited.size() == rooms.size()) return true;  // all rooms visited in this dfs already

        if (room_keys[room] == 0) return false;
        if (visited.find(room) != visited.end()) return false;

        visited.insert(room);
        if (visited.size() == rooms.size()) return true;
        
        // I entered this room, pick up my keys.
        for (int key : rooms[room]) {
            room_keys[key]++;
        }

        // Visit possibly new rooms
        for (int r : rooms[room]) {
            if (dfs(r)) return true;
        }

        // No need to give up room keys or unvisit the room
        // As long as the dfs "touches" a room, we are good
        return false;
    }
public:
    bool canVisitAllRooms(vector<vector<int>>& rooms_in) {
        rooms = rooms_in;
        room_keys = vector<int>(rooms.size(), 0);
        room_keys[0] = 1;  // I always have this key
        return dfs(0);
    }
};
