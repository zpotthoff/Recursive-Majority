#include <queue>
#include <cmath>
#include <unordered_set>
#include <iostream>
#include <bitset>
#include "tree.h"

Tree::Tree() {
    this->numVars = VARS;
    this->root = nullptr;
    this->rng = new RandomGenerator();
    this->treeType = "";
}

Tree::Tree(const Tree& other) {
    this->numVars = other.numVars;
    this->root = new Node(*other.root);
    this->rng = new RandomGenerator();
    this->treeType = other.treeType;
}

Tree::~Tree() {
    delete this->rng;
    deleteSubtree(this->root);
}

void Tree::clear() {
    deleteSubtree(this->root);
    this->root = nullptr;
}

void Tree::deleteSubtree(Node* curr) {
    if (curr == nullptr)
        return;
    if (curr->getLeft() == nullptr && curr->getRight() == nullptr) {
        delete curr;
        return;
    }
    else if (curr->getRight() == nullptr) {
        deleteSubtree(curr->getLeft());
        delete curr;
        return;
    }
    else if (curr->getLeft() == nullptr) {
        deleteSubtree(curr->getRight());
        delete curr;
        return;
    }
    else {
        deleteSubtree(curr->getLeft());
        deleteSubtree(curr->getRight());
        delete curr;
        return;
    }
}

std::string Tree::getTreeType() {
    return this->treeType;
}

void Tree::levelOrderTraversal() {
    privLevelOrderTraversal(this->root);
}

void Tree::privLevelOrderTraversal(Node* treeRoot) {
    if (treeRoot == nullptr) 
        return;
    std::queue<Node*> q;
    q.push(treeRoot);
    q.push(new Node(-99,{}));
    while (q.size() > 1) {
        Node* node = q.front();
        q.pop();
        if (node->getVar() == -99) {
            delete node; // delete the -99
            q.push(new Node(-99,{}));
            std::cout << std::endl;
        }
        else {
            std::cout << node->getVar() << ",";
            if (node->getLeft() != nullptr)
                q.push(node->getLeft());
            if (node->getRight() != nullptr) 
                q.push(node->getRight());
        }
    }
    Node* node = q.front();
    delete node; // delete the last -99
    std::cout << std::endl;
}

void Tree::traversalToFile(std::ofstream& file) {
    Node* treeRoot = this->root;
    if (treeRoot == nullptr) 
        return;

    std::queue<Node*> q;
    q.push(treeRoot);
    q.push(new Node(-99,{}));
    while (q.size() > 1) {
        Node* node = q.front();
        q.pop();
        if (node->getVar() == -99) {
            delete node; // delete the -99
            q.push(new Node(-99,{}));
            file << "\n";
        }
        else {
            file << node->getVar() << ",";
            if (node->getLeft() != nullptr)
                q.push(node->getLeft());
            if (node->getRight() != nullptr) 
                q.push(node->getRight());
        }
    }
    Node* node = q.front();
    delete node; // delete the last -99
}

float Tree::getCost(std::string filename) {
    std::ifstream file(filename);
    float ret = privGetCost(file);
    file.close();
    return ret;
}

float Tree::privGetCost(std::ifstream& file) {
    std::string inputString;
    int total = 0, i = 0;
    Node* curr;
    while (std::getline(file, inputString)) {
        curr = this->root;
        std::bitset<VARS> bitString(inputString);
        if (this->getTreeType() != "Complete") {
            if (bitString[this->root->getVar()]== 0)
            continue;
        }
        while (curr->getVar() != -LEAF_VAL && curr->getVar() != LEAF_VAL){
            if (bitString[curr->getVar()] == 0) {
                curr = curr->getLeft();
            }
            else {
                curr = curr->getRight();
            }
        }
        if (curr->getVar() != -LEAF_VAL && curr->getVar() != LEAF_VAL) {
            throw std::runtime_error("ERROR: something in cost");
        }
        i++;
        total += (curr->getComputed() & std::bitset<BSSIZE>((pow(2, BSSIZE)-1) - (pow(2, BSSIZE-VARS)-1))).count();
    }
    return (float) total / i;
}

// temperature defaulted to -1
void Tree::reconstructRandomBranch(double temperature) {
    privReconstructRandomBranch(temperature);
}

void Tree::privReconstructRandomBranch(double temperature) {
    // random number between 0 and 2^VARS - 1
    int randomNum = rng->randomInt(pow(2,this->numVars));
    std::bitset<BSSIZE> randomPath(randomNum);
    if (treeType != "Complete") { // only an optimization for Evaluate algorithms
        randomPath.set(this->root->getVar(), 1); // always go to the subtree that exists
    }
    int nodes = 0;
    Node* curr = this->root;
    while (curr->getVar() != -LEAF_VAL && curr->getVar() != LEAF_VAL) {
        if (randomPath[curr->getVar()] == 0) {
            curr = curr->getLeft();
        }
        else {
            curr = curr->getRight();
        }
        nodes++;
    }
    int destroyNode = 0;
    while (destroyNode == 0) { // don't destroy the root
        if (temperature == -1) {
            destroyNode = rng->randomInt(nodes);
        }
        else {
            destroyNode = rng->randomBetaInt(nodes, temperature);
        }
    }

    // if we're doing eval and want to preserve top 3 nodes
    if (this->getTreeType() == "Eval") {
        if (destroyNode < 3) 
            return;
    }
    curr = this->root;

    while (destroyNode > 0) {
        if (randomPath[curr->getVar()] == 0) {
            curr = curr->getLeft();
        }
        else {
            curr = curr->getRight();
        }
        destroyNode--;
    }
    deleteSubtree(curr->getLeft());
    deleteSubtree(curr->getRight());

    std::bitset<BSSIZE> leftComputed = curr->getComputed();
    std::bitset<BSSIZE> leftValues = curr->getBitValues();
    Maj* leftMaj = new Maj(leftComputed, leftValues);
    leftMaj->setbit(curr->getVar(), 0);
    curr->setLeft(privBuildRandomSubtree(leftMaj));

    std::bitset<BSSIZE> rightComputed = curr->getComputed();
    std::bitset<BSSIZE> rightValues = curr->getBitValues();
    Maj* rightMaj = new Maj(rightComputed, rightValues);
    rightMaj->setbit(curr->getVar(), 1);
    curr->setRight(privBuildRandomSubtree(rightMaj));
}

// temperature defaulted to -1
void Tree::levelReconstruct(double temperature) {
    privLevelReconstruct(temperature);
}

void Tree::privLevelReconstruct(double temperature) {
    int level = 0;
    int possibleLevels;
    if (this->getTreeType() == "Complete") {
        possibleLevels = ((2*VARS) / 3) - 1;
    } else {
        possibleLevels = VARS - 1;
    }

    // do not allow to reconstruct the root
    while (level == 0) {
        // random num between 0 and VARS - 2 b/c it wouldn't make sense 
        // to reconstruct the last variable since we just rebuild it

        if (temperature == -1) {
            // NOTE: currently adjusted for the smaller "complete" function tree
            level = this->rng->randomInt(possibleLevels);
        } else {
            level = this->rng->randomBetaInt(possibleLevels, temperature);
        }
        if (this->getTreeType() == "Eval") { // ensures we don't destroy the top 3 levels
            if (level < 3) {
                level = 0;
            }
        }
    }
    int randomNum, destroyNode;
    std::bitset<BSSIZE> randomPath;
    bool pathFound = false;
    Node* curr = this->root;

    while (!pathFound) {
        randomNum = rng->randomInt(pow(2,this->numVars));
        randomPath = std::bitset<BSSIZE>(randomNum);
        if (treeType != "Complete") {
            randomPath.set(this->root->getVar(), 1);
        }
        destroyNode = 0;
        curr = this->root;
        while (curr->getVar() != -LEAF_VAL && curr->getVar() != LEAF_VAL) {
            if (destroyNode == level) {
                pathFound = true;
                break;
            }
            if (randomPath[curr->getVar()] == 0) {
                curr = curr->getLeft();
            }
            else {
                curr = curr->getRight();
            }
            destroyNode++;
        }
    }
    deleteSubtree(curr->getLeft());
    deleteSubtree(curr->getRight());

    std::bitset<BSSIZE> leftComputed = curr->getComputed();
    std::bitset<BSSIZE> leftValues = curr->getBitValues();
    Maj* leftMaj = new Maj(leftComputed, leftValues);
    leftMaj->setbit(curr->getVar(), 0);
    curr->setLeft(privBuildRandomSubtree(leftMaj));

    std::bitset<BSSIZE> rightComputed = curr->getComputed();
    std::bitset<BSSIZE> rightValues = curr->getBitValues();
    Maj* rightMaj = new Maj(rightComputed, rightValues);
    rightMaj->setbit(curr->getVar(), 1);
    curr->setRight(privBuildRandomSubtree(rightMaj));
}

bool Tree::smartQuery(std::bitset<BSSIZE> computed, int var) {
    if (var < 0 || var >= VARS) {
        throw std::runtime_error("ERROR: invalid variable");
    }
    int pos = var + BSSIZE - VARS; // position in the bitset
    while (pos > 0) {
        if (computed[pos] == 1) { // some ancestor has been computed, so don't want to query var
            return false;
        }
        pos = (pos - 1) / 3; // go to parent
    }
    return computed[0] == 0; // root has not been computed
}