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
         * Private helper that builds a subtree with random nodes of variables 
         * given the ancestors that came before that node. Ensures 
         * that ancestors will not be selected as the node value. 
         * The ancestors are given in the form of a Maj object.
         * @param subtreeMaj the majority object to be used as ancestors in the subtree
         * @return Node pointer to the root of the subtree
        */
        Node* privBuildRandomSubtree(Maj* subtreeMaj);

        /**
         * Private helper that prints out a level order traversal given 
         * the root of the subtree to print out. Nodes are comma separated.
         * -999999 indicates that the represented query algorithm would
         * output a 0 at that point and 999999 indicates a 1 would be 
         * outputted.
         * @param treeRoot the root of the tree to traverse
        */
        void privLevelOrderTraversal(Node* treeRoot);
        
        /**
         * Private helper to calculate the expected cost of a random 
         * 3-MAJ query algorithm. The algorithm must already exist 
         * as a Tree object. A file of bitstrings is passed in to 
         * calculate the cost. Some input file are found in 
         * the Input-Distributions directory.
         * @param file the file to read the bitstrings from
         * @return the expected cost of the algorithm
        */
        float privGetCost(std::ifstream& file);

        /**
         * Private helper to build an "easy" subtree. An "easy" 
         * tree is defined such that it is the left child is the 
         * lowest node value available (no repeated nodes on a given 
         * path) and the right child is the highest node value available.
         * A Maj object is used to determine the ancestors and a character 
         * input determine which subtree the current node is building from.
         * @param subtreeMaj the majority object to be used as 
         * ancestors in the subtree
         * @param whichChild the character input to determine which subtree 
         * a node is building from its parent. 'l' denotes left, 'r' denotes 
         * right.
         * @return Node pointer to the root of the subtree
         */
        Node* privBuildEasySubtree(Maj* subtreeMaj, char whichChild);

        /**
         * Private helper to build the naïve random tree. This function builds 
         * the "base" of the tree, making calls to build the B-section and C-section 
         * of the tree.
         * @return Node pointer to the root of the tree
         */
        Node* privBuildNaiveRandomTree();

        /**
         * Private helper to build the B-section of the naïve random tree. 
         * The B-section is defined as the algorithm that evaluates the 
         *  middle child of the root. Three 
         * parameters are given, two to determine what is done after this section 
         * is built, and one to indicate the ancestors of this branch.
         * @param leftChild char indicating what is done if the B-section evaluates 
         * to 0. 'c' to build the C-section, anything else to terminate and output 
         * because 3-majority has been calculated
         * @param rightChild same as `leftChild`
         * @param branchMaj the majority object to be used as ancestors in the subtree
         * @return Node pointer to the root of the B-section
         */
        Node* privBuildNaiveB(char leftChild, char rightChild, Maj* branchMaj);

        /** 
         * Private helper to build the C-section of the naïve random tree.
         * The C-section is defined as the algorithm that evaluates the 
         * right child of the root. Guaranteed to output if this section 
         * is executed as we will have queried each of the root's children.
         * @param branchMaj the majority object to be used as ancestors in the subtree
         */
        Node* privBuildNaiveC(Maj* branchMaj);

        /**
         * Private function that destroys a subtree given a pointer to a 
         * subtree root. Cleans up the memory of the deleted subtree.
         */
        void deleteSubtree(Node* curr);

        /** 
         * Private function that reconstructs a random branch of the tree. 
         * Temperature is taken into account if the Beta distribution 
         * is being used, not if node selection is uniform. 
         */
        void privReconstructRandomBranch(double temperature);

        /**
         * Private helper to build the Eval Random Tree. 
         * @return Node pointer to the root of the tree
         */
        Node* privBuildEvalRandomTree();

        /** 
         * Private helper to reconstruct the algorithm by selecting 
         * a level at random, then selecting a node from within that 
         * level. Distribution the level is selected from is determined 
         * by the temperature as given from public function 
         * `levelReconstruct()`.
         * @param temperature the temperature of simulated annealing, which 
         * adjusts the Beta distribution. Defaulted to -1 for uniform 
         * level selection
         */
        void privLevelReconstruct(double temperature);

        /**
         * Private helper to build the A-section of MNSSTX algorithm. 
         * Reference will soon be in README.md about what this represents.
         */
        Node* privMNSSTX_A(Maj* branchMaj);
        
        /**
         * Private helper to build the B-section of MNSSTX algorithm. 
         * Reference will soon be in README.md about what this represents.
         */
        Node* privMNSSTX_B(Maj* branchMaj);

        /**
         * Private helper to build the C-section of MNSSTX algorithm. 
         * Reference will soon be in README.md about what this represents.
         */
        Node* privMNSSTX_C(Maj* branchMaj);

        /**
         * Private helper to build the D-section of MNSSTX algorithm. 
         * Reference will soon be in README.md about what this represents.
         */
        Node* privMNSSTX_D(Maj* branchMaj);

        /**
         * Private function that determines if a potential next query is 
         * a "smart" one, which is defined as one that contributes towards 
         * getting an answer for the root. A query that is not smart is one 
         * where we query a node whose ancestors have already had their 
         * value computed, meaning we do not gain information from the query.
         * @param computed bitset of which nodes in the tree have already been 
         * computed
         * @param var the value of the node of the possible query
         * @return `TRUE` if this query is smart, `FALSE` if not
         */
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
         * Function that prints out a level-order traversal given 
         * the root of the subtree to print out. Nodes are comma separated.
         * -999999 indicates that the represented query algorithm would
         * output a 0 at that point and 999999 indicates a 1 would be 
         * outputted.
        */
        void levelOrderTraversal();

        /**
         * Function that sends a level-order traversal in the same style as 
         * `levelOrderTraversal()` to a given file.
         * @param file an opened file to send a level-order traversal to. 
         * User is responsible for closing the file.
         */
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
         * Builds the tree representing the naïve random algorithm. The naïve random 
         * algorithm was proposed in 1986 by Michael Saks and Avi Wigderson. 
         * It is defined such that one of the root's children is selected 
         * at random and recursively evaluated. Another child is then randomly 
         * selected and recursively evaluated. If the two children's values 
         * are the same, the algorithm returns that value. If not, the third 
         * child is evaluated and returned. The deterministic version of the 
         * algorithm is built here and when the hard distribution is permuted 
         * over this tree, the expected cost matches and we have a representation 
         * of the algorithm. Currently only constructed to work for depth-2 trees 
         * (9 input variables).
         */
        void buildNaiveRandomTree();

        /**
         * Function that clears the tree and cleans memory, EXCEPT for the tree 
         * object itself. The object still exists, but none of the tree remains.
         */
        void clear();

        void reconstructRandomBranch(double temperature = -1);

        /**
         * Builds the tree representing the Eval Random algorithm. The Eval Random
         * algorithm is built from the idea of using the naïve algorithm to 
         * compute one child at random (at depth-2 this is the algorithm with least 
         * expected cost). Then, the rest of the queries are made at random.
         * Only constructed currently to work for depth-2 trees (9 input variables).
         * While simulated annealing is running, it is ensured that the nodes queried 
         * from that first child will not be reconstructed.
         */
        void buildEvalRandomTree();

        /** 
         * Function to reconstruct the algorithm by selecting 
         * a level at random, then selecting a node from within that 
         * level. The level can be chosen uniformly if `temperature` = -1
         * or according to the Beta distribution otherwise.
         * @param temperature the temperature of simulated annealing, which 
         * adjusts the Beta distribution. Defaulted to -1 for uniform 
         * level selection
         */
        void levelReconstruct(double temperature = -1);

        /**
         * Function that builds the state of the art, depth-2 3-MAJ 
         * algorithm published in 2016 by Frederic Magniez, Ashwin Nayak, Miklos Santha, 
         * Jonah Sherman, Gabor Tardos, and David Xiao. Similar to `buildNaiveRandomTree()`,
         * this is a deterministic representation of the randomized algorithm. When 
         * all permutations of hard input are presented to the algorithm, it acts as a 
         * randomized algorithm. This function makes calls to build subsections of the 
         * algorithm.
         */
        void buildMNSSTX();

        /**
         * Function that builds a random algorithm representing a "Complete" subroutine.
         * Complete is a subroutine that evaluates the root, given the value of one of 
         * its children. NOTE: Complete does not mean complete in the Complete tree sense, 
         * it refers to the subroutine "Complete."
         */
        void buildComplete();

        /**
         * @return the string of the type of algorithm being represented by the tree
         */
        std::string getTreeType();

    private:
        // Tree(const Tree& other) = delete;
        Tree& operator=(const Tree& other) = delete;
};