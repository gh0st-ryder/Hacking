
class Solution {
    // This problem is some grade A+ level bullshit in my honest opinion.
    //
    // The original code I wrote below will work if and only if
    // The candies are unique
    // The bags are ALSO unique
    // Then the combinatorics I computed works out fine. 
    //
    // However as stated in this problem
    // The candies are unique
    // But the bags are NOT unique
    // 
    // Basically this property reminds one of the math involved in Bose-Einstein condensates
    // which basically modifies the combinatorics because two photons are indistinguishable from each other for example
    // (Here two bags are like two photons)
    //
    // To compute the same problem using BEC math, we just have to know about 
    // Stirling numbers of the second kind
    //
    // Yeah.. fuck me, I guess?
    //
    // I mean maybe I could have solved it without using Stirling numbers if I took my original approach
    // and divided by the number of ways to arrange the bags? (bags factorial?)
    // IDK... too lazy to test this theory
    //
    // I left my original code in place for the modified problem (bags also unique)
    // And solved the given problem (bags NOT unique) using Stirling numbers of the second kind.
     
    long long MODULO = 1000000007;

    // hash tabel to memorize n `choose` m.
    unordered_map<int, long long> n_c_m;

    int key_n_m(int n, int m) {
        return ((n << 16) | (m));
    }

    // memoized n `choose` m
    long long n_choose_m(int n, int m) {
        int key = key_n_m(n, m);
        // printf("n=%d, m=%d; key=%d\n", n, m, key);
        if (n_c_m.find(key) != n_c_m.end()) {
            return n_c_m[key];
        }
        long long ans=1;
        for (int i=n; i>=n-m+1; i--) {
            ans *= i;
            ans %= MODULO;
        }
        n_c_m[key] = ans;
        return ans;
    }

    // hash table to memoize combinations
    unordered_map<int, long long> cache;

    long long ways(int candies, int bags) {
        printf("Solving for %d, %d\n", candies, bags);
        if (bags == 1) return candies;
        if (candies < bags) return 0;
        if (candies == bags) return 1;
        int key = key_n_m(candies, bags);
        if (cache.find(key) != cache.end()) {
            return cache[key];
        }
        long long ans=0;
        int my_candies_limit = std::max(1, candies);
        for (int my_candies=1; my_candies <= candies-bags+1; my_candies++) {
            // For myself, I can pick "my_candies" number of candies in "my_picked" number of ways.
            long long my_picked = n_choose_m(candies, my_candies);

            // Solve the problems for bags after mine.
            long long rec = ways(candies-my_candies, bags-1);

            printf("[%d, %d] => my_candies = %d, my_picked = %lld, rec = %lld\n", candies, bags, my_candies, my_picked, rec);

            // How many ways for this sub-problem.
            ans = my_picked * rec;
            ans %= MODULO;
        }
        cache[key] = ans;
        return ans;
    }

    long long stirling(int n, int m) {
        if (m == 1) return 1;
        if (m == n) return 1;
        int key = key_n_m(n, m);
        if (cache.find(key) != cache.end()) return cache[key];

        long long ans = m * stirling(n-1, m) + stirling(n-1, m-1);
        ans  %= MODULO;
        cache[key] = ans;
        return ans;
    }

public:
    int waysToDistribute(int n, int k) {
        return stirling(n, k);
    }
};
