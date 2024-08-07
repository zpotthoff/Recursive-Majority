#include <string>
#include <iostream>
#include <fstream>
#include <climits>
#include <filesystem>
#include <chrono>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include "tree.h"
#include "node.h"
#include "randomGenerator.h"
#include "globals.h"

int main() {

    std::cout << "Complete or Evaluate? \n\t1. Evaluate\n\t2. Complete\n";
    int algorithmType;
    std::cin >> algorithmType;
    std::string distributionFilename, outputPath = "Output-Trees/";
    Tree* t = new Tree();
    if (algorithmType == 1) {
        distributionFilename = "Input-Distributions/hardDistribution" + std::to_string(VARS) + ".txt";
        std::cout << "Tree Structure? \n\t1. Random\n\t2. Easy\n\t3. Naive Random\n\t4. Eval Random\n\t5. MNSSTX\n";
        int treeStructure;
        std::cin >> treeStructure;
        if (treeStructure == 1) {
            t->buildRandomTree();
        } else if (treeStructure == 2) {
            t->buildEasyTree();
        } else if (treeStructure == 3) {
            t->buildNaiveRandomTree();
        } else if (treeStructure == 4) {
            t->buildEvalRandomTree();
        } else if (treeStructure == 5) {
            t->buildMNSSTX();
        } else {
            throw std::runtime_error("Invalid tree structure");
        }
        outputPath += "Evaluate/";
    } else if (algorithmType == 2) {
        distributionFilename = "Input-Distributions/hardDistributionComplete" + std::to_string((2 * VARS) / 3) + ".txt";
        t->buildComplete();
        outputPath += "Complete/";
    } else {
        throw std::runtime_error("Invalid tree type");
    }

    float coolingRate;
    std::cout << "Cooling Rate: ";
    std::cin >> coolingRate;
    if (coolingRate <= 0 || coolingRate >= 1) {
        throw std::runtime_error("Invalid cooling rate");
    }

    float temperatureFloor;
    std::cout << "Temperature Floor: ";
    std::cin >> temperatureFloor;
    if (temperatureFloor <= 0) {
        throw std::runtime_error("Invalid temperature floor");
    }

    int distributionType;
    std::cout << "Distribution Type: \n\t1. Uniform\n\t2. Beta\n";
    std::cin >> distributionType;
    if (distributionType != 1 && distributionType != 2) {
        throw std::runtime_error("Invalid distribution type");
    }
    if (distributionType == 1) {
        outputPath += "Uniform-";
    } else {
        outputPath += "Beta-";
    }

    int reconstructionType;
    std::cout << "Reconstruction Type: \n\t1. Path\n\t2. Level\n";
    std::cin >> reconstructionType;
    if (reconstructionType != 1 && reconstructionType != 2) {
        throw std::runtime_error("Invalid reconstruction type");
    }
    if (reconstructionType == 1) {
        outputPath += "Path/";
    } else {
        outputPath += "Level/";
    }

    auto start = std::chrono::high_resolution_clock::now();
    RandomGenerator* rng = new RandomGenerator();
    Tree* t2;
    Tree* bestTree = new Tree(*t);

    int invalidCount = 0, i = 0;
    float bestCost = t->getCost(distributionFilename), newCost;
    float initTemperature = 1;
    float temperature = initTemperature;

    while (temperature > temperatureFloor) {
        t2 = new Tree(*t);
        if (reconstructionType == 1) {  
            if (distributionType == 1) {
                t2->reconstructRandomBranch();
            } else {
                t2->reconstructRandomBranch(temperature);
            }
        } else {
            if (distributionType == 1) {
                t2->levelReconstruct();
            } else {
                t2->levelReconstruct(temperature); 
            }
        }

        newCost = t2->getCost(distributionFilename);
        float acceptanceProbability = exp(-(newCost - bestCost) / temperature);
        if (acceptanceProbability > (float)(rng->randomInt(INT_MAX) / INT_MAX)) {
            delete t;
            t = t2;
        } else {
            delete t2;
        }
        if (newCost < bestCost) {
            delete bestTree;
            bestTree = new Tree(*t);
            bestCost = newCost;
        }
        temperature *= coolingRate;
        if (i % 1000 == 0) {
            std::cout << "currently on iteration " << i << ", best cost: " << bestCost << std::endl;
        }
        i++;
    }
    
    int numFiles = 0;
    for (const auto & entry : std::filesystem::directory_iterator(outputPath)) {
        numFiles++;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Invalid Trees: " << invalidCount << std::endl;
    std::cout << "Iterations: " << i << std::endl;
    std::cout << "Best Cost: " << bestCost << std::endl;

    std::stringstream ofString;
    ofString << outputPath << "C" << std::fixed << std::setprecision(6) << std::to_string((pow(bestCost, 1.0 / HEIGHT)) - 2).substr(2) << 
                "D" << std::to_string(HEIGHT) << "-" << std::to_string(numFiles+1) << ".txt";
    std::ofstream outFile(ofString.str());
    outFile << "Depth: " << HEIGHT << std::endl;
    outFile << "Cost: " << bestCost << std::endl;
    outFile << "Recursion Base: " << pow(bestCost, 1.0 / HEIGHT) << std::endl;
    outFile << "Invalid Count: " << invalidCount << std::endl;
    outFile << "Starting Tree: " << bestTree->getTreeType() << std::endl;
    outFile << "Initial Temperature: " << initTemperature << std::endl;
    outFile << "Cooling Rate: " << coolingRate << std::endl;
    outFile << "Temperature Floor: " << temperatureFloor << std::endl;
    outFile << "Time: " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " sec" << std::endl;
    bestTree->traversalToFile(outFile);

    outFile.close();
    delete t;
    delete bestTree;
    delete rng;
    return 0;
}