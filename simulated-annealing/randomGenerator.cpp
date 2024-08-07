#include "randomGenerator.h"
#include <random>
#include <chrono>

RandomGenerator::RandomGenerator() {
    //generate one-time seed
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine tmp(seed); //store generator in tmp var
    this->generator = tmp; //assign generator to tmp
}

int RandomGenerator::randomInt(int size) {
    // the line below produces a *probability distribution* uniform on {0,...,size-1}
    // so we can use this as a random index generation
    std::uniform_int_distribution<int> distribution(0,size-1);
    // this line actually generates the random int.
    int z = distribution(generator);
    return z;
}

int RandomGenerator::randomBetaInt(int maxVal, double temperature) {
    // Adjust alpha and beta based on temperature
    // For simplicity, let's fix beta at 2.0 and adjust alpha based on temperature
    // Higher temperature -> lower alpha for skew towards lower values
    // Lower temperature -> higher alpha for skew towards higher values
    double alpha = 5 / (1.35+temperature); // Example adjustment, can be modified
    double beta = 5 / (1.35- temperature); // Fixed beta

    // Create a Beta distribution
    std::gamma_distribution<double> distAlpha(alpha, 1.0);
    std::gamma_distribution<double> distBeta(beta, 1.0);

    double aSample = distAlpha(this->generator);
    double bSample = distBeta(this->generator);
    double betaSample = aSample / (aSample + bSample);

    // Scale the Beta distribution sample to [0, maxVal)
    int discreteValue = static_cast<int>(betaSample * (maxVal));

    return discreteValue;
}