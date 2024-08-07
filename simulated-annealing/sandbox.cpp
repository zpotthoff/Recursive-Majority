#include "randomGenerator.h"
#include "tree.h"
#include <vector>
#include <iostream>
#include <unistd.h>
#include <chrono>

int main() {
    // RandomGenerator* rng = new RandomGenerator();
    // std::vector<int> test(28,0);
    // for (float z = 1; z >= 0.001; z *= .8) {
    //     system("clear");
    //     std::cout << "Temperature: " << z << std::endl;
    //     test.clear();
    //     test.resize(28, 0);
    //     for (int i = 0; i < 500; i++) {
    //         test[rng->randomBetaInt(28, z)]++;
    //     }
    //     for (int i = 0; i < 28; i++) {
    //         std::cout << i << ": ";
    //         for (int j = 0; j < test[i]; j++) {
    //             std::cout << "*";
    //         }
    //         std::cout << std::endl;
    //     }
    //     sleep(1);
    // }


    // const auto start = std::chrono::high_resolution_clock::now();
    // Tree* t = new Tree();
    // t->buildEasyTree();
    // const auto end = std::chrono::high_resolution_clock::now();
    // std::cout << "Time to build tree: " << std::chrono::duration<double> (end - start).count() << "s" << std::endl;

    // const auto start2 = std::chrono::high_resolution_clock::now();
    // t->getCost("Input-Distribtions/hardDistribution27.txt");
    // const auto end2 = std::chrono::high_resolution_clock::now();
    // std::cout << "Time to get cost: " << std::chrono::duration<double> (end2 - start2).count() << "s" << std::endl;

    // Tree* t = new Tree();
    // t->buildMNSSTX();
    // std::cout << t->getCost("Input-Distributions/hardDistribution9.txt") << std::endl;
    // t->levelOrderTraversal();

    Tree* t = new Tree();
    t->buildComplete();
    t->levelOrderTraversal();
    std::cout << t->getCost("Input-Distributions/hardDistribution9.txt") << std::endl;
    delete t;
    return 0;
}