class Solution {
    // The recurrence relation
    // ibreak(1) = 1
    // ibreak(p) = (max over j=1 to p/2) (ibreak(j) * ibreak(p-j))
public:
    int integerBreak(int n) {
        vector<long long> ibreak(n+1, 1);
        for (int p=2; p<=n; p++) {  // the nums go from 2 to n
            long long maxprod = 1;
            for (int j=1; j<=p/2; j++) {    
                printf("Trying for %d and %d indices\n", j, p-j);            
                long long prod = ibreak[j] * ibreak[p-j];
                if (prod > maxprod) {
                    maxprod = prod;
                }
            }
            // another maxprod            
            ibreak[p] = std::max(maxprod, (long long)p);
        }
        // Special case the following, because in these, the number itself is not the right choice to max above.
        // Once we cross 4, the number itself will NEVER be the right choice.
        if (n == 2) return 1;
        if (n == 3) return 2;
        if (n == 4) return 4;
        return ibreak[n];
    }
};
