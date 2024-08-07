#pragma once

#include <vector>
#include <string>
#include <fstream>
#include "node.h"
#include "maj.h"
#include "randomGenerator.h"
#include "globals.h"

class Tree {
    private:
        int numVars;
        Node* root;
        RandomGenerator* rng;
        std::string treeType;
        
        /**
         * Helper function that builds a subtree in a bad way. A
         * random number from the variable list is placed in every 
         * node position. NOTE: It is not guaranteed that this is an 
         * invalid tree, but it is likely.
         * @param nodeAncestors the ancestors of a given node
         * @return A pointer to the Node the subtree is rooted at
        */
        Node* privBuildBadSubtree(std::bitset<VARS> nodeAncestors);

        /**
         * Private helper that builds a subtree with random nodes of variables 
         * given the ancestors that came before that node. Ensures 
         * that ancestors will not be selected as the node value.
         * @param subtreeMaj the majority object to be used in the subtree
         * @return Node pointer to the root of the subtree
        */
        Node* privBuildRandomSubtree(Maj* subtreeMaj);

        /**
         * Private helper that prints out a level order traversal given 
         * the root of the subtree to print out.
         * @param treeRoot the root of the tree to traverse
        */
        void privLevelOrderTraversal(Node* treeRoot);

        /**
         * Private helper that validates a tree represents a query algorithm
         * for the 3-MAJ problem.
         * Checks that all paths are the same length as the number of variables 
         * and elements in each path are unique.
         * @param curr the current node in the path
         * @param path the path thus far (excluding `curr`)
         * @return `true` if tree is a valid representation of a query algorithm,
         * `false` otherwise
        */
        bool privValid3MAJAlgo(Node* curr, std::vector<int> path);

                
        float privGetCost(std::ifstream& file);

        Node* privBuildEasySubtree(Maj* subtreeMaj, char whichChild);

        /**
         * Private helper to build the naive random tree. 
         */
        Node* privBuildNaiveRandomTree();

        Node* privBuildNaiveB(char leftChild, char rightChild, Maj* branchMaj);

        Node* privBuildNaiveC(Maj* branchMaj);

        void deleteSubtree(Node* curr);

        void privReconstructRandomBranch(double temperature);

        /**
         * Exact same as build naive tree, except instead of going to 
         * buildB, it goes to buildRandomSubtree.
         */
        Node* privBuildEvalRandomTree();

        void privLevelReconstruct(double temperature);

        Node* privMNSSTX_A(Maj* branchMaj);
        
        Node* privMNSSTX_B(Maj* branchMaj);

        Node* privMNSSTX_C(Maj* branchMaj);

        Node* privMNSSTX_D(Maj* branchMaj);

        bool smartQuery(std::bitset<BSSIZE> computed, int var);

    public:
        /**
         * Constructor to create a tree structure.
        */
        Tree();

        /**
         * Copy constructor for the tree.
         * @param other the tree to copy
        */
        Tree(const Tree& other);

        ~Tree();

        /**
         * Function to build a bad tree structure. NOTE: it is not guaranteed 
         * that the tree is bad, but it is very likely as tree size increases.
        */
        void buildBadTree();

        /**
         * Function to build a random query algorithm in the form 
         * of a tree representation. All variables should be provided.
         * @param varList vector of ints of all the variables included
         * in the query algorithm
        */
        void buildRandomTree();

        /**
         * Prints out a level order traveral of the tree.
        */
        void levelOrderTraversal();

        void traversalToFile(std::ofstream& file);

        /**
         * Checks that the tree is a valid representation of a query algorithm 
         * for the 3-MAJ problem.
         * @return `true` if tree is a valid representation of a query algorithm,
         * `false` otherwise
        */
        bool valid3MAJAlgo();

        /**
         * Function to calculate the expected cost of a tree given a file of bitstrings.
         * @param filename the file to read the bitstrings from
         * @return the expected cost of the tree
        */
        float getCost(std::string filename);

        /**
         * Builds an easy-to-check tree. Works such that the earliest non-queried 
         * bit is queried. If it is 0, go to the next earliest non-queried bit. If 
         * it is a 1, go to the farthest back non-queried bit.
         */
        void buildEasyTree();

        /**
         * Builds the naive random tree.
         */
        void buildNaiveRandomTree();

        void clear();

        void reconstructRandomBranch(double temperature = -1);

        void buildEvalRandomTree();

        void levelReconstruct(double temperature = -1);

        void buildMNSSTX();

        void buildComplete();

        std::string getTreeType();

    private:
        // Tree(const Tree& other) = delete;
        Tree& operator=(const Tree& other) = delete;
};