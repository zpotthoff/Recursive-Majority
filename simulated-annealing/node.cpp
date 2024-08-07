#include "node.h"
#include "globals.h"

Node::Node(int variableName, Maj* maj) {
    this->var = variableName;
    this->maj = maj;
    this->left = nullptr;
    this->right = nullptr;
}

Node::Node(int variableName, Maj* maj, Node* left, Node* right) {
    this->var = variableName;
    this->maj = maj;
    this->left = left;
    this->right = right;
}

Node::Node(const Node& other) {
    this->var = other.var;
    this->maj = new Maj(*other.maj);
    if (this->var == LEAF_VAL || this->var == -LEAF_VAL) {
        this->left = nullptr;
        this->right = nullptr;
        return;
    }
    if (other.left != nullptr)
        this->left = new Node(*other.left);
    else 
        this->left = nullptr;
    this->right = new Node(*other.right);
}

Node::~Node() {
    delete this->maj;
}

void Node::setVar(int variableName) {
    this->var = variableName;
}

void Node::setLeft(Node* newLeft) {
    this->left = newLeft;
}

void Node::setRight(Node* newRight) {
    this->right = newRight;
}

int Node::getVar() {
    return this->var;
}

Node* Node::getLeft() {
    return this->left;
}

Node* Node::getRight() {
    return this->right;
}

std::bitset<BSSIZE> Node::getComputed() {
    return this->maj->getComputed();
}

std::bitset<BSSIZE> Node::getBitValues() {
    return this->maj->getBitValues();
}

bool Node::majIsComputed() {
    return this->maj->isComputed();
}