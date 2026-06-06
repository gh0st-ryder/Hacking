struct Info {
    char ch;  // the letter
    int freq;  // the unconsumed freq    
};

std::function<bool(const Info&, const Info&)> comp = [](const Info& lhs, const Info& rhs) -> bool {
    if (lhs.freq != rhs.freq) return lhs.freq < rhs.freq;  // higher freq on top of the heap
    return lhs.ch < rhs.ch;
};

using MaxHeap=std::priority_queue<Info, vector<Info>, decltype(comp)>;

class Solution {
    MaxHeap mh;
    
public:
    string longestDiverseString(int a, int b, int c) {
        mh = MaxHeap(comp);
        if (a > 0) {            
            mh.push({'a', a});
        }
        if (b > 0) {            
            mh.push({'b', b});
        }
        if (c > 0) {            
            mh.push({'c', c});
        }

        string str;
        while (!mh.empty()) {
            Info info_top = mh.top();
            mh.pop();            
            
            if (str.size() < 2 || (!(str[str.size()-1] == info_top.ch && str[str.size()-2] == info_top.ch))) {
                // Will process just the one letter.                
                str.push_back(info_top.ch);
                if (info_top.freq-1 != 0) {                    
                    mh.push({info_top.ch, info_top.freq-1});
                }                
            } else {
                // Pull down the next one too
                if (mh.empty()) return str;  // we are done.
                Info info_top_2 = mh.top();
                mh.pop();
                str.push_back(info_top_2.ch);                
                if (info_top_2.freq-1 != 0) {                    
                    mh.push({info_top_2.ch, info_top_2.freq-1});
                }                
                mh.push(info_top);
            }
        }
        return str;        
    }
};
