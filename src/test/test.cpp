#include <iostream>
#include <cassert> // For the assert() function
#include <cmath>   // For std::abs
#include "../math/DiceMath.h"

// Helper function to compare floating-point numbers (double)
// We cannot use "==" with doubles due to floating-point precision errors
bool isClose(double a, double b, double epsilon = 0.0001) {
    return std::abs(a - b) < epsilon;
}

int main() {
    std::cout << "Starting math engine tests...\n";

    // --- TEST 1: Random Generator (DiceRoller) ---
    DiceRoller roller;
    int roll = roller.rollDice(20);
    assert(roll >= 1 && roll <= 20); // The d20 must be between 1 and 20
    assert(roller.rollDice(-5) == 0); // Safety check for non-existent dice

    // --- TEST 2: DicePool Setup ---
    DicePool pool;
    pool.addDices(2, 6); // 2d6
    pool.setModifier(3); // 2d6 + 3
    
    // Test that rollTotal respects physical limits (min 5, max 15)
    for(int i = 0; i < 100; ++i) {
        // --- MODIFICA QUI: Aggiungi .total alla fine ---
        int total = pool.rollTotal(roller).total; 
        assert(total >= 5 && total <= 15);
    }

    // --- TEST 3: Expected Value and Standard Deviation ---
    // Mean of 1d6 = 3.5 -> 2d6 = 7.0 -> +3 modifier = 10.0
    assert(isClose(pool.getExpectedValue(), 10.0));
    
    // Variance of 1d6 = (36-1)/12 = 2.9166. Variance of 2d6 = 5.8333.
    double expectedStdDev = std::sqrt(5.83333333);
    assert(isClose(pool.getStandardDeviation(), expectedStdDev));

    // --- TEST 4: Exact Analytical Distribution ---
    Distribution dist = pool.getExactDistribution();
    
    // The minimum possible value with 2d6 + 3 is 5
    assert(dist.minValue == 5); 
    
    // Impossible probabilities must be 0
    assert(isClose(dist.getProbabilityOf(4), 0.0)); 
    assert(isClose(dist.getProbabilityOf(16), 0.0));
    
    // In 2d6, the probability of rolling a 7 is 6/36. With the +3, it means getting a 10.
    assert(isClose(dist.getProbabilityOf(10), 6.0 / 36.0));
    
    // Probability of rolling AT LEAST 14 (requires an 11 or 12 on the dice) = (2/36) + (1/36) = 3/36
    assert(isClose(dist.getProbabilityAtLeast(14), 3.0 / 36.0));

    // If we reach this point without the program crashing, all tests passed!
    std::cout << "SUCCESS! All math tests passed successfully.\n";

    return 0;
}
