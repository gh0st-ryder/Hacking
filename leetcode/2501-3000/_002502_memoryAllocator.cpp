class Allocator {
    enum State {FREE=0, ALLOC=1};
    struct StateInfo {
        State state;
        int mID;  // The mID that this block belongs to
    };
    struct AllocInfo {
        int mID;    // The mID for the allocation
        int start;  // the start block
        int size;   // the size of the allocation
    };
    int N;
    // Key: block_id; Value: The state of the block
    unordered_map<int, StateInfo> memory;

    // Key: mID; Value: A vector of AllocInfo for all allocations to mID not freed yet.
    unordered_map<int, vector<AllocInfo>> allocations;

    int findFree(int mID, int size) {        
        for (int i=0; i<N; i++) {            
            int count=0;
            while (i < N && count < size && memory[i].state == FREE) {
                i++;
                count++;
            }
            if (count == size) {
                int start = i - size;
                for (int j=start; j<start+size; j++) {
                    memory[j] = {ALLOC, mID};
                }                
                return start;
            }
        }        
        return -1;
    }

public:
    Allocator(int n) {
        N = n;
        for (int i=0; i<N; i++) {
            memory[i] = {FREE, -1};
        }
    }
    
    int allocate(int size, int mID) {
        // Find a hole of size "size"                    
        int next_i = findFree(mID, size);
        if (next_i == -1) return -1;
        allocations[mID].push_back({mID, next_i, size});        
        return next_i;           
    }
    
    int freeMemory(int mID) {
        if (allocations.find(mID) == allocations.end()) return 0;
        int total=0;
        for (AllocInfo info : allocations[mID]) {          
            total += info.size;
            for (int i=info.start; i<info.start + info.size; i++) {
                memory[i] = {FREE, -1};
            }            
        }
        allocations.erase(mID);
        return total;
    }
};

/**
 * Your Allocator object will be instantiated and called as such:
 * Allocator* obj = new Allocator(n);
 * int param_1 = obj->allocate(size,mID);
 * int param_2 = obj->freeMemory(mID);
 */
