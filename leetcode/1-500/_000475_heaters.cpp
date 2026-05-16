class Solution {
    vector<int> houses_, heaters_;

    bool coversAllHouses(int radius) {
        unordered_set<int> covered; // indices of houses covered so far
        int heater_ptr=0;  // the index of the heater we will now cover
        int house_ptr=0;  // the index of the house to now check.

        // Two-pointers
        // We fix heater_ptr, and advance house_ptr rightwards until it goes out of bounds of the last heater
        // Then we advance the heater_ptr ahead by one
        // continue until either all houses are covered, or we can't cover a house with a heater

        while (house_ptr < houses_.size() && heater_ptr < heaters_.size() && covered.size() != houses_.size()) {
            // Cover as many houses as we can with this heater.
            while (house_ptr < houses_.size() && std::abs(houses_[house_ptr] - heaters_[heater_ptr]) <= radius) {
                covered.insert(house_ptr++);
            }
            if (covered.size() == houses_.size()) {
                return true;  // all houses covered
            }
            // Advance to the next heater.
            heater_ptr++;
        }
        return (covered.size() == houses_.size());
    }

    int binSearchRadius(int radius_low, int radius_high) {
        if (radius_low == radius_high) {            
            return radius_high;
        }
        if (radius_low+1 == radius_high) {
            if (coversAllHouses(radius_low)) {
                return radius_low;
            }
            return radius_high;
        }
        int mid = (radius_low + radius_high)/2;
        bool mid_covers = coversAllHouses(mid);
        if (mid_covers) {return binSearchRadius(radius_low, mid);}
        return binSearchRadius(mid, radius_high);
    }

public:
    int findRadius(vector<int>& houses, vector<int>& heaters) {
        houses_ = houses;
        heaters_ = heaters;
        std::sort(houses_.begin(), houses_.end());
        std::sort(heaters_.begin(), heaters_.end());
        return binSearchRadius(0, std::max(houses_.back(), heaters_.back()));
    }
};
