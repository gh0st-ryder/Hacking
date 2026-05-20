class Solution {
    // TODO: Need to work on this more thoroughly when I have more time
    //
    // Each button can be flipped a certain number of times from [0..presses]
    // Assume each button is flipped n1, n2, n3, n4 number of times.
    // The additional constraint is that (n1+n2+n3+n4=presses) exactly.
    //
    // Button 1 only affects the outcome if it is flipped an odd number of times
    //    and in this case, it flips everything once
    // Button 2 only affects the outcome if it is flipped an odd number of times
    //    and in this case, it flips only the even bulbs
    // Button 3 only affects the outcome if it is flipped an odd number of times
    //    and in this case, it flips only the odd bulbs
    // Button 4 only affects the outcome if it is flipped an odd number of times
    //    and in this case, it flips only the bulbs in the sequence 1, 4, 7, 10...
    //
    // So the question is, can I divide up "presses" into "n1, n2, n3, n4" such that
    // each of n1, n2, n3, n4 <- [odd, even]
    // In fact, if I can do "odd, even, even, even" then I can do any combination of 
    // 1 odd, 3 evens
    // and so on.
    //
    // So I find whether I can do:
    // Exactly 1 odd => 4 choose 1 combinations
    // Exactly 2 odd => 4 choose 2 combinations
    // Exactly 3 odd => 4 choose 3 combinations
    // Exactly 4 odd => 4 choose 4 combinations
    // I can treat the "even" as 0 flips for even numbered flips
    //
    // So can I divide up "presses" into exactly 1, 2, 3, 4 odds is the question.
    //
    // the number of bulbs only matter in so much as whether they make a certain button matter
    // For example, with n=1, only the first bulb matters, so the second button is a no-op

    // Returns the number of states when exactly one button is picked to be odd number of flips.
    int statesFromOneOdd(int n, int presses) {
        if (presses % 2 == 0) {
            return 0;
        }
        if (n == 1) return 1;  // button 1, 3, or 4 is odd and return the same state
        if (n == 2) return 2;  // flip 1 or flip 2
        if (n == 3) return 3;  // flip 1, or 2, or 3
        return 4;  // either the state formed from buttons 1/3/4 or the state from button 2.
    }

    int statesFromTwoOdd(int n, int presses) {

    }
public:
    int flipLights(int n, int presses) {
        if (presses == 0) return 1;
    }
};
