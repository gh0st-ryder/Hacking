struct Info {
    int key;
    int timestamp;  // used to keep freshness info since we don't have a delete in the heap
    bool operator==(const Info& rhs) const {
        return (key == rhs.key && timestamp == rhs.timestamp);
    }
};

template<>
struct hash<Info> {
    size_t operator()(const Info& info) const {
        return (size_t)((info.key) | (info.timestamp << 16));
    }
};

std::function<bool(const Info&, const Info&)> comp = [](const Info& lhs, const Info& rhs) -> bool {
    if (lhs.timestamp != rhs.timestamp) return lhs.timestamp > rhs.timestamp;  // LRU, smallest timestamp on top of heap
    return lhs.key < rhs.key;  // doesn't matter, every key has unique timestamp
};

using MinHeap = priority_queue<Info, vector<Info>, decltype(comp)>;

class LRUCache {
    int timestamp=0;
    int CAPACITY=0;
    // Key: key; Value: value
    unordered_map<int, int> cache;

    // Key: key; Value: the timestamp for the only valid incarnation of this key
    // Used to lazily "delete" older entries from the heap 
    unordered_map<int, int> latest;

    // Heap for LRU.
    MinHeap heap;

    // forcibly evicts the LRU item from the cache.
    void evict() {        
        while (!heap.empty()) {
            Info info = heap.top(); heap.pop();
            if (latest[info.key] > info.timestamp) {
                // Stale entry for key, discard and try again
                continue;
            }
            assert(latest[info.key] == info.timestamp);
            cache.erase(info.key);
            latest.erase(info.key);
            break;
        }
    }
public:
    LRUCache(int capacity) {
        heap = MinHeap(comp);
        CAPACITY = capacity;
    }
    
    int get(int key) {
        if (cache.find(key) == cache.end()) return -1;
        // if it's not in the cache, we can't get it. No eviction needed.
        // If it is in the cache, bumping its timestamp is enough, no eviction needed
        // Bump the LRU timestamp
        latest[key] = timestamp;
        heap.push({key, timestamp});
        timestamp++;        
        return cache[key];
    }
    
    void put(int key, int value) {
        if (cache.find(key) != cache.end()) {
            // Found in teh cache; don't need to evict anything; just refresh timestamp
            ;
        } else {
            if (cache.size() == CAPACITY) {
                evict();
            }
        }
        cache[key] = value;
        latest[key] = timestamp;
        heap.push({key, timestamp});
        timestamp++;           
    }
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */
