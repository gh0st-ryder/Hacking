class PhoneDirectory {
    unordered_set<int> registered_;
    std::deque<int> holes_;
    int next_num_=0;
    int max_numbers_;
public:
    PhoneDirectory(int maxNumbers) {
        max_numbers_ = maxNumbers;
    }
    
    int get() {
        int number=0;
        if (!holes_.empty()) {
            number = holes_.front();
            holes_.pop_front();
        } else if (next_num_ == max_numbers_) {
            return -1;
        } else {
            number = next_num_++;
        }
        registered_.insert(number);
        return number;
    }
    
    bool check(int number) {
        return registered_.find(number) == registered_.end();
    }
    
    void release(int number) {
        if (registered_.find(number) != registered_.end()) {
            registered_.erase(number);
            holes_.push_back(number);
        }
    }
};

/**
 * Your PhoneDirectory object will be instantiated and called as such:
 * PhoneDirectory* obj = new PhoneDirectory(maxNumbers);
 * int param_1 = obj->get();
 * bool param_2 = obj->check(number);
 * obj->release(number);
 */
