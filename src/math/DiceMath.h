#pragma once

#include <vector>
#include <random>

// ============================================================================
// DATA STRUCTURES
// ============================================================================

// Holds the exact probability distribution (PMF) of a dice pool
struct Distribution {
    int minValue;
    std::vector<double> probabilities;

    // Returns the exact probability of hitting a specific target exactly
    double getProbabilityOf(int target) const;
    
    // Returns the probability of hitting AT LEAST a target (e.g., DC 15)
    double getProbabilityAtLeast(int target) const;
};

// Represents a homogeneous group of dice (e.g., 2d6)
struct DiceGroup {
    int quantity;
    int faces;
};

// --- NUOVA STRUCT PER I RISULTATI ---
struct RollResult {
    int total;
    std::vector<int> individualRolls;
};

// ============================================================================
// DICE ROLLER ENGINE
// ============================================================================

// Core random number generator. Instantiate only once and pass by reference.
class DiceRoller {
private:
    std::mt19937 rng;

public:
    DiceRoller(); 
    int rollDice(int n_faces); 
};


// ============================================================================
// DICE POOL LOGIC
// ============================================================================

// Represents a complex roll formula (e.g., 2d6 + 1d8 + 3)
class DicePool {
private:
    std::vector<DiceGroup> groups;
    int modifier;

public:
    DicePool();
    
    void addDices(int quantity, int faces);
    void setModifier(int mod);
    void clear();
    
    RollResult rollTotal(DiceRoller& roller) const;

    double getExpectedValue() const;
    double getStandardDeviation() const;
    Distribution getExactDistribution() const;
    std::string getFormulaText() const;
};
