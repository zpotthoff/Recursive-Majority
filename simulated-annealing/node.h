#pragma once
#include <string>
#include <vector>
#include <bitset>
#include "maj.h"
#include "globals.h"

class Node {
    private:
        int var;
        Maj* maj;
        Node* left;
        Node* right;
    
    public:
        /**
         * Constructor primarily used for leaves, assigns left and 
         * right child as nullptr's.
         * @param variableName integer of which variable you are setting
         * @param maj object of node's majority information
        */
        Node(int variableName, Maj* maj);
        /**
         * Constructor for internal nodes.
         * @param variableName integer of which variable you are setting
         * @param maj object of node's majority information
         * @param left pointer to left child node
         * @param right pointer to right child node
        */
        Node(int variableName, Maj* maj, Node* left, Node* right);

        /**
         * Copy constructor for the node.
         * @param other the node to copy
        */
        Node(const Node& other);

        /**
         * Destructor for the node.
        */
        ~Node();
        
        /**
         * Changes the variable the node represents.
         * @param variableName the new variable the node represents
        */
        void setVar(int variableName);

        /**
         * Changes the left child of the current node.
         * @param newLeft the new left child of the current node
        */
        void setLeft(Node* newLeft);

        /**
         * Changes the right child of the current node.
         * @param newRight the new right child of the current node
        */
        void setRight(Node* newRight);

        /**
         * Gets the name of the variable the current node represents.
         * @return an integer of the variable the node represents
        */
        int getVar();

        /**
         * Gets the left child of the current node
         * @return Node pointer to left child
        */
        Node* getLeft();

        /**
         * Gets the right child of the current node
         * @return Node pointer to right child
        */
        Node* getRight();

        std::bitset<BSSIZE> getComputed();

        std::bitset<BSSIZE> getBitValues();

        bool majIsComputed();

    private:
        // Node(const Node& other) = delete;
        Node& operator=(const Node& other) = delete;
};