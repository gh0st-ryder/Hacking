class Solution {
    double EPSILON = 0.00001;
public:
    bool judgeSquareSum(int c) {
        int a = 0;
        while (true) {
            int check = c - a*a;
            if (check < 0) break;
            double b = std::sqrt(check);
            int b_int = static_cast<int>(b);
            if (b_int < a) {
                break;  // already checked this.
            }
            if ((b - (double)(b_int)) < EPSILON) {  // floating point denormals beware?
                printf("Found a=%d, b=%d for c=%d\n", a, b_int, c);
                return true;
            }
            a++;
        }
        return false;
    }
};
