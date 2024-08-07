#pragma once
#include <random>

class RandomGenerator {
    private:
        std::default_random_engine generator;
    public:
        /**
         * Constructor that creates a seed and generator to work with.
        */
        RandomGenerator();

        /**
         * Function that generates a random int uniformly on range 
         * [0, `size`-1].
         * @param size the size of the range of random number generation
         * @return Uniform random integer on range [0,`size`-1] 
        */
        int randomInt(int size);

        /**
         * Function generates a random int according to approximated discrete 
         * Beta distribution on [0, maxVal]. Higher temperature skews towards low values and 
         * lower temperature skews towards high values.
         * @param maxVal the maximum value of the distribution
         * @param temperature the temperature of simulated annealing
         * @return Random int according to approximated discrete Beta distribution
         */
        int randomBetaInt(int maxVal, double temperature);
};