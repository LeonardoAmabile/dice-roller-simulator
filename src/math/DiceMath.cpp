#include "DiceMath.h" 
#include <cmath> // Required for std::sqrt

// ============================================================================
// INTERNAL HELPERS (Anonymous namespace)
// ============================================================================
namespace {
    // Exact discrete convolution of two independent distributions
    Distribution convolve(const Distribution& a, const Distribution& b) {
        Distribution result;
        
        // The new minimum value is the sum of the minimums
        result.minValue = a.minValue + b.minValue;
        
        // The new size is size(A) + size(B) - 1
        size_t newSize = a.probabilities.size() + b.probabilities.size() - 1;
        result.probabilities.assign(newSize, 0.0); // Initialize with 0s

        // Multiply the polynomials
        for (size_t i = 0; i < a.probabilities.size(); ++i) {
            for (size_t j = 0; j < b.probabilities.size(); ++j) {
                result.probabilities[i + j] += a.probabilities[i] * b.probabilities[j];
            }
        }
        
        return result;
    }
} // end anonymous namespace


// ============================================================================
// DISTRIBUTION METHODS
// ============================================================================

double Distribution::getProbabilityOf(int target) const {
    int index = target - minValue;
    if (index >= 0 && index < probabilities.size()) {
        return probabilities[index];
    }
    return 0.0; // Impossible result
}

double Distribution::getProbabilityAtLeast(int target) const {
    double totalProb = 0.0;
    for (size_t i = 0; i < probabilities.size(); ++i) {
        if (minValue + static_cast<int>(i) >= target) {
            totalProb += probabilities[i];
        }
    }
    return totalProb;
}


// ============================================================================
// DICEROLLER METHODS
// ============================================================================

DiceRoller::DiceRoller() {
    std::random_device dev;
    rng.seed(dev());
}

int DiceRoller::rollDice(int n_faces) {
    if (n_faces < 1) return 0;
    std::uniform_int_distribution<int> dist(1, n_faces);
    return dist(rng);
}


// ============================================================================
// DICEPOOL METHODS - Setup
// ============================================================================

DicePool::DicePool() : modifier(0) {}

void DicePool::addDices(int quantity, int faces) {
    if (quantity > 0 && faces > 1) {
        // Cerca se esiste già un gruppo con le stesse facce
        for (auto& group : groups) {
            if (group.faces == faces) {
                group.quantity += quantity; // Unisce i dadi!
                return; // Esce dalla funzione
            }
        }
        // Se non lo ha trovato, aggiunge un nuovo gruppo
        groups.push_back({quantity, faces});
    }
}

void DicePool::setModifier(int mod) {
    modifier = mod;
}

void DicePool::clear() {
    groups.clear();
    modifier = 0;
}


// ============================================================================
// DICEPOOL METHODS - Rolling & Math
// ============================================================================

RollResult DicePool::rollTotal(DiceRoller& roller) const {
    RollResult result;
    result.total = modifier;
    
    for (const auto& group : groups) {
        for (int i = 0; i < group.quantity; ++i) {
            int roll = roller.rollDice(group.faces);
            result.individualRolls.push_back(roll); 
            result.total += roll;                   
        }
    }
    
    return result;
}

double DicePool::getExpectedValue() const {
    double expectedValue = modifier;
    
    for (const auto& group : groups) {
        // Mean of a single dX is (X + 1) / 2.0
        double singleDieMean = (group.faces + 1) / 2.0;
        expectedValue += group.quantity * singleDieMean;
    }
    
    return expectedValue;
}

double DicePool::getStandardDeviation() const {
    double totalVariance = 0.0;
    
    for (const auto& group : groups) {
        // Variance of a single dX is (X^2 - 1) / 12.0
        double singleDieVariance = (group.faces * group.faces - 1) / 12.0;
        totalVariance += group.quantity * singleDieVariance; // Independent events add up
    }
    
    return std::sqrt(totalVariance); // StdDev is square root of variance
}

Distribution DicePool::getExactDistribution() const {
    // Base case: starting value is just the modifier (100% chance)
    Distribution currentDist;
    currentDist.minValue = modifier;
    currentDist.probabilities = { 1.0 }; 

    for (const auto& group : groups) {
        // Create the distribution for a SINGLE die of this group
        Distribution singleDie;
        singleDie.minValue = 1;
        singleDie.probabilities.assign(group.faces, 1.0 / group.faces);

        // Convolve it for as many dice as we have in this group
        for (int i = 0; i < group.quantity; ++i) {
            currentDist = convolve(currentDist, singleDie);
        }
    }

    return currentDist;
}

// ============================================================================
// DICEPOOL METHODS - Pool name 
// ============================================================================
std::string DicePool::getFormulaText() const {
    if (groups.empty() && modifier == 0) return "Vuoto";

    std::string formula = "";
    bool first = true;

    // Cicla tutti i dadi e costruisce la stringa (es. "2d6 + 1d8")
    for (const auto& group : groups) {
        if (!first) formula += " + ";
        formula += std::to_string(group.quantity) + "d" + std::to_string(group.faces);
        first = false;
    }

    // Aggiunge il modificatore se presente
    if (modifier > 0) {
        if (!first) formula += " + ";
        formula += std::to_string(modifier);
    } else if (modifier < 0) {
        if (!first) formula += " - ";
        else formula += "-";
        // Uso -modifier per evitare di stampare " + -3" o "- -3"
        formula += std::to_string(-modifier);
    }

    return formula;
}
