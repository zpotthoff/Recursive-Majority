#include "tree.h"

void Tree::buildRandomTree() {
    this->treeType = "Random";
    Maj* rootMaj = new Maj();
    this->root = new Node(0, rootMaj);
    Maj* rightMaj = new Maj(*rootMaj);
    rightMaj->setbit(0, 1);
    this->root->setRight(privBuildRandomSubtree(rightMaj));
}

Node* Tree::privBuildRandomSubtree(Maj* subtreeMaj) {
    Node* curr = nullptr;
    //base case
    if (subtreeMaj->isComputed()) {
        if (subtreeMaj->getValue() == 0)
            curr = new Node(-LEAF_VAL, subtreeMaj);
        else
            curr = new Node(LEAF_VAL, subtreeMaj);
        return curr;
    }
    std::bitset<BSSIZE> variableLocations((pow(2, BSSIZE) - 1) - (pow(2, BSSIZE - VARS) - 1));
    int numZeros = VARS - (subtreeMaj->getComputed() & variableLocations).count();
    int randIndex, selectedVar;

    do {
        randIndex = rng->randomInt(numZeros);
        // i iterates over the bitset, j iterates over leaf variables to keep track of selectedVar
        for (int i = BSSIZE-VARS, j = 0; i < BSSIZE; i++, j++) {
            if (subtreeMaj->getComputed()[i] == 0) {
                if (randIndex == 0) {
                    // if (j % 3 == 2) { // we are on third bit of gate
                    //     if (subtreeMaj->getComputed()[i-1] == 0) { // if second bit hasn't been computed
                    //         j--; // leaf var
                    //         i--; // var with respect to maj bitset
                    //     }
                    // }
                    // if (j % 3 == 1) { // we are on second bit of gate
                    //     if (subtreeMaj->getComputed()[i-1] == 0) { // if first bit hasn't been computed
                    //         j--;
                    //         i--;
                    //     }
                    // }
                    selectedVar = j;
                    break;
                }
                randIndex--;
            }
        }
    } while (!this->smartQuery(subtreeMaj->getComputed(), selectedVar));

    curr = new Node(selectedVar, subtreeMaj);

    Maj* leftMaj = new Maj(*subtreeMaj);
    leftMaj->setbit(selectedVar, 0);
    curr->setLeft(privBuildRandomSubtree(leftMaj));

    Maj* rightMaj = new Maj(*subtreeMaj);
    rightMaj->setbit(selectedVar, 1);
    curr->setRight(privBuildRandomSubtree(rightMaj));

    return curr;
}

void Tree::buildEasyTree() {
    this->treeType = "Easy";
    Maj* rootMaj = new Maj();
    this->root = new Node(0, rootMaj);

    Maj* rightMaj = new Maj();
    rightMaj->setbit(0, 1);
    this->root->setRight(privBuildEasySubtree(rightMaj,'r'));
}

Node* Tree::privBuildEasySubtree(Maj* subtreeMaj, char whichChild) {
    Node* curr = nullptr;

    //base case, if a majority has been computed, stop building tree, put either 
    // -LEAF_VAL or +LEAF_VAL as the node value
    if (subtreeMaj->isComputed()) {
        if (subtreeMaj->getValue() == 0)
            curr = new Node(-LEAF_VAL, subtreeMaj);
        else
            curr = new Node(LEAF_VAL, subtreeMaj);
        return curr;
    }
    int selectedVar;
    if (whichChild == 'l') {
        for (int i = BSSIZE-VARS, j = 0; i < BSSIZE; i++, j++) {
            if (subtreeMaj->getComputed()[i] == 0) {
                selectedVar = j; // we do conversions in Maj class
                break;
            }
        }
    }
    else {
        for (int i = BSSIZE - 1, j = VARS - 1; i >= BSSIZE - VARS; i--, j--) {
            if (subtreeMaj->getComputed()[i] == 0) {
                selectedVar = j;
                break;
            }
        }
    }
    
    curr = new Node(selectedVar, subtreeMaj);
    
    Maj* leftMaj = new Maj(*subtreeMaj);
    leftMaj->setbit(selectedVar, 0);
    curr->setLeft(privBuildEasySubtree(leftMaj,'l'));

    Maj* rightMaj = new Maj(*subtreeMaj);
    rightMaj->setbit(selectedVar, 1);
    curr->setRight(privBuildEasySubtree(rightMaj,'r'));

    return curr;
}

void Tree::buildNaiveRandomTree() {
    this->treeType = "Naive";
    privBuildNaiveRandomTree();
}

Node* Tree::privBuildNaiveRandomTree() {
    Maj* rootMaj = new Maj();
    this->root = new Node(0, rootMaj);

    std::bitset<BSSIZE> rComputed = rootMaj->getComputed();
    std::bitset<BSSIZE> rValues = rootMaj->getBitValues();
    Maj* rMaj = new Maj(rComputed, rValues);
    rMaj->setbit(0, 1);
    Node* rChild = new Node(1, rMaj);
    this->root->setRight(rChild);

    std::bitset<BSSIZE> rlComputed = rChild->getComputed();
    std::bitset<BSSIZE> rlValues = rChild->getBitValues();
    Maj* rlMaj = new Maj(rlComputed, rlValues);
    rlMaj->setbit(1, 0);
    Node* rlChild = new Node(2, rlMaj);
    rChild->setLeft(rlChild);

    Maj* rrMaj = new Maj(rChild->getComputed(), rChild->getBitValues());
    rrMaj->setbit(1, 1);
    rChild->setRight(privBuildNaiveB('c','o', rrMaj));

    Maj* rllMaj = new Maj(rlChild->getComputed(), rlChild->getBitValues());
    rllMaj->setbit(2, 0);
    rlChild->setLeft(privBuildNaiveB('z', 'c', rllMaj));
    
    Maj* rlrMaj = new Maj(rlChild->getComputed(), rlChild->getBitValues());
    rlrMaj->setbit(2, 1);
    rlChild->setRight(privBuildNaiveB('c','o', rlrMaj));

    return this->root;
}

Node* Tree::privBuildNaiveB(char leftChild, char rightChild, Maj* branchMaj) {
    
    Node* bRoot = new Node(3, branchMaj);

    Maj* lMaj = new Maj(*branchMaj);
    lMaj->setbit(3, 0);
    Node* lChild = new Node(4, lMaj);
    bRoot->setLeft(lChild);

    Maj* rMaj = new Maj(*branchMaj);
    rMaj->setbit(3, 1);
    Node* rChild = new Node(4, rMaj);
    bRoot->setRight(rChild);

    Maj* lrMaj = new Maj(*lMaj);
    lrMaj->setbit(4, 1);
    Node* lrChild = new Node(5, lrMaj);
    lChild->setRight(lrChild);

    Maj* rlMaj = new Maj(*rMaj);
    rlMaj->setbit(4, 0);
    Node* rlChild = new Node(5, rlMaj);
    rChild->setLeft(rlChild);

    Maj* llMaj = new Maj(*lMaj);
    llMaj->setbit(4, 0);
    Maj* lrlMaj = new Maj(*lrMaj);
    lrlMaj->setbit(5, 0);
    Maj* rllMaj = new Maj(*rlMaj);
    rllMaj->setbit(5, 0);
    if (leftChild == 'c') {
        
        lChild->setLeft(privBuildNaiveC(llMaj));
        lrChild->setLeft(privBuildNaiveC(lrlMaj));
        rlChild->setLeft(privBuildNaiveC(rllMaj));
    }
    else {
        lChild->setLeft(new Node(-LEAF_VAL, llMaj));
        lrChild->setLeft(new Node(-LEAF_VAL, lrlMaj));
        rlChild->setLeft(new Node(-LEAF_VAL, rllMaj));
        if (!lChild->getLeft()->majIsComputed() || !lrChild->getLeft()->majIsComputed() || !rlChild->getLeft()->majIsComputed()) {
            throw std::runtime_error("ERROR: maj not computed B1");
        }
    }

    Maj* rrMaj = new Maj(*rMaj);
    rrMaj->setbit(4, 1);
    Maj* lrrMaj = new Maj(*lrMaj);
    lrrMaj->setbit(5, 1);
    Maj* rlrMaj = new Maj(*rlMaj);
    rlrMaj->setbit(5, 1);
    if (rightChild == 'c') {
        rChild->setRight(privBuildNaiveC(rrMaj));
        lrChild->setRight(privBuildNaiveC(lrrMaj));
        rlChild->setRight(privBuildNaiveC(rlrMaj));
    }
    else {
        rChild->setRight(new Node(LEAF_VAL, rrMaj));
        lrChild->setRight(new Node(LEAF_VAL, lrrMaj));
        rlChild->setRight(new Node(LEAF_VAL, rlrMaj));
        if (!rChild->getRight()->majIsComputed() || !lrChild->getRight()->majIsComputed() || !rlChild->getRight()->majIsComputed()) {
            throw std::runtime_error("ERROR: maj not computed B2");
        }
    }

    return bRoot;
}

Node* Tree::privBuildNaiveC(Maj* branchMaj) {
    Node* cRoot = new Node(6, branchMaj);

    Maj* lMaj = new Maj(*branchMaj);
    lMaj->setbit(6, 0);
    Node* lChild = new Node(7, lMaj);
    cRoot->setLeft(lChild);

    Maj* rMaj = new Maj(*branchMaj);
    rMaj->setbit(6, 1);
    Node* rChild = new Node(7, rMaj);
    cRoot->setRight(rChild);

    Maj* lrMaj = new Maj(*lMaj);
    lrMaj->setbit(7, 1);
    Node* lrChild = new Node(8, lrMaj);
    lChild->setRight(lrChild);

    Maj* llMaj = new Maj(*lMaj);
    llMaj->setbit(7, 0);
    lChild->setLeft(new Node(-LEAF_VAL, llMaj));
    if (!lChild->getLeft()->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed C1");
    }

    Maj* rlMaj = new Maj(*rMaj);
    rlMaj->setbit(7, 0);
    Node* rlChild = new Node(8, rlMaj);
    rChild->setLeft(rlChild);

    Maj* rrMaj = new Maj(*rMaj);
    rrMaj->setbit(7, 1);
    rChild->setRight(new Node(LEAF_VAL, rrMaj));
    if (!rChild->getRight()->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed C2");
    }

    Maj* lrlMaj = new Maj(*lrMaj);
    lrlMaj->setbit(8, 0);
    lrChild->setLeft(new Node(-LEAF_VAL, lrlMaj));
    Maj* lrrMaj = new Maj(*lrMaj);
    lrrMaj->setbit(8, 1);
    lrChild->setRight(new Node(LEAF_VAL, lrrMaj));
    if (!lrChild->getLeft()->majIsComputed() || !lrChild->getRight()->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed C3");
    }

    Maj* rllMaj = new Maj(*rlMaj);
    rllMaj->setbit(8, 0);
    rlChild->setLeft(new Node(-LEAF_VAL, rllMaj));
    Maj* rlrMaj = new Maj(*rlMaj);
    rlrMaj->setbit(8, 1);
    rlChild->setRight(new Node(LEAF_VAL, rlrMaj));
    if (!rlChild->getLeft()->majIsComputed() || !rlChild->getRight()->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed C4");
    }

    return cRoot;
}

void Tree::buildEvalRandomTree() {
    this->treeType = "Eval";
    this->root = privBuildEvalRandomTree();
}

Node* Tree::privBuildEvalRandomTree() {
    Maj* rootMaj = new Maj();
    this->root = new Node(0, rootMaj);

    std::bitset<BSSIZE> rComputed = rootMaj->getComputed();
    std::bitset<BSSIZE> rValues = rootMaj->getBitValues();
    Maj* rMaj = new Maj(rComputed, rValues);
    rMaj->setbit(0, 1);
    Node* rChild = new Node(1, rMaj);
    this->root->setRight(rChild);

    std::bitset<BSSIZE> rlComputed = rChild->getComputed();
    std::bitset<BSSIZE> rlValues = rChild->getBitValues();
    Maj* rlMaj = new Maj(rlComputed, rlValues);
    rlMaj->setbit(1, 0);
    Node* rlChild = new Node(2, rlMaj);
    rChild->setLeft(rlChild);

    Maj* rrMaj = new Maj(rChild->getComputed(), rChild->getBitValues());
    rrMaj->setbit(1, 1);
    rChild->setRight(privBuildRandomSubtree(rrMaj));

    Maj* rllMaj = new Maj(rlChild->getComputed(), rlChild->getBitValues());
    rllMaj->setbit(2, 0);
    rlChild->setLeft(privBuildRandomSubtree(rllMaj));
    
    Maj* rlrMaj = new Maj(rlChild->getComputed(), rlChild->getBitValues());
    rlrMaj->setbit(2, 1);
    rlChild->setRight(privBuildRandomSubtree(rlrMaj));

    return this->root;
}

void Tree::buildMNSSTX() {
    this->treeType = "MNSSTX";

    // variable names follow format r for right, l for left, follow sequence from root
    // first declare all nodes, then connect them

    // create nodes

    //level 0
    Maj* rootMaj = new Maj();
    this->root = new Node(0, rootMaj);

    //level 1
    Maj* rMaj = new Maj(*rootMaj);
    rMaj->setbit(0, 1);
    Node* rChild = new Node(3, rMaj);

    //level 2
    Maj* rlMaj = new Maj(*rMaj);
    rlMaj->setbit(3, 0);
    Node* rlChild = new Node(6, rlMaj);

    Maj* rrMaj = new Maj(*rMaj);
    rrMaj->setbit(3, 1);
    Node* rrChild = new Node(1, rrMaj);

    //level 3
    Maj* rllMaj = new Maj(*rlMaj);
    rllMaj->setbit(6, 0);
    Node* rllChild = new Node(7, rllMaj);

    Maj* rlrMaj = new Maj(*rlMaj);
    rlrMaj->setbit(6, 1);
    Node* rlrChild = new Node(7, rlrMaj);

    Maj* rrlMaj = new Maj(*rrMaj);
    rrlMaj->setbit(1, 0);
    Node* rrlChild = new Node(2, rrlMaj);

    Maj* rrrMaj = new Maj(*rrMaj);
    rrrMaj->setbit(1, 1);
    Node* rrrChild = privMNSSTX_C(rrrMaj);

    //level 4
    Maj* rlllMaj = new Maj(*rllMaj);
    rlllMaj->setbit(7, 0);
    Node* rlllChild = this->privMNSSTX_A(rlllMaj);

    Maj* rllrMaj = new Maj(*rllMaj);
    rllrMaj->setbit(7, 1);
    Node* rllrChild = new Node(8, rllrMaj);

    Maj* rlrlMaj = new Maj(*rlrMaj);
    rlrlMaj->setbit(7, 0);
    Node* rlrlChild = new Node(8, rlrlMaj);

    Maj* rlrrMaj = new Maj(*rlrMaj);
    rlrrMaj->setbit(7, 1);
    Node* rlrrChild = this->privMNSSTX_B(rlrrMaj);

    Maj* rrllMaj = new Maj(*rrlMaj);
    rrllMaj->setbit(2, 0);
    Node* rrllChild = this->privMNSSTX_D(rrllMaj);

    Maj* rrlrMaj = new Maj(*rrlMaj);
    rrlrMaj->setbit(2, 1);
    Node* rrlrChild = this->privMNSSTX_C(rrlrMaj);

    // level 5
    Maj* rllrlMaj = new Maj(*rllrMaj);
    rllrlMaj->setbit(8, 0);
    Node* rllrlChild = this->privMNSSTX_A(rllrlMaj);

    Maj* rllrrMaj = new Maj(*rllrMaj);
    rllrrMaj->setbit(8, 1);
    Node* rllrrChild = this->privMNSSTX_B(rllrrMaj);

    Maj* rlrllMaj = new Maj(*rlrlMaj);
    rlrllMaj->setbit(8, 0);
    Node* rlrllChild = this->privMNSSTX_A(rlrllMaj);

    Maj* rlrlrMaj = new Maj(*rlrlMaj);
    rlrlrMaj->setbit(8, 1);
    Node* rlrlrChild = this->privMNSSTX_B(rlrlrMaj);

    // connections 

    // level 0
    this->root->setRight(rChild);

    // level 1
    rChild->setLeft(rlChild); rChild->setRight(rrChild);

    // level 2
    rlChild->setLeft(rllChild); rlChild->setRight(rlrChild);
    rrChild->setLeft(rrlChild); rrChild->setRight(rrrChild);

    // level 3
    rllChild->setLeft(rlllChild); rllChild->setRight(rllrChild);
    rlrChild->setLeft(rlrlChild); rlrChild->setRight(rlrrChild);
    rrlChild->setLeft(rrllChild); rrlChild->setRight(rrlrChild);
    
    // level 4
    rllrChild->setLeft(rllrlChild); rllrChild->setRight(rllrrChild);
    rlrlChild->setLeft(rlrllChild); rlrlChild->setRight(rlrlrChild);
}

Node* Tree::privMNSSTX_A(Maj* branchMaj) {
    // create nodes

    //level 0
    Node* aRoot = new Node(4, branchMaj);

    // level 1
    Maj* lMaj = new Maj(*branchMaj);
    lMaj->setbit(4, 0);
    Node* lChild = new Node(-LEAF_VAL, lMaj);
    if (!lChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed A1");
    }

    Maj* rMaj = new Maj(*branchMaj);
    rMaj->setbit(4, 1);
    Node* rChild = new Node(5, rMaj);

    // level 2
    Maj* rlMaj = new Maj(*rMaj);
    rlMaj->setbit(5, 0);
    Node* rlChild = new Node(-LEAF_VAL, rlMaj);
    if (!rlChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed A2");
    }

    Maj* rrMaj = new Maj(*rMaj);
    rrMaj->setbit(5, 1);
    Node* rrChild = new Node(1, rrMaj);

    // level 3
    Maj* rrlMaj = new Maj(*rrMaj);
    rrlMaj->setbit(1, 0);
    Node* rrlChild = new Node(2, rrlMaj);

    Maj* rrrMaj = new Maj(*rrMaj);
    rrrMaj->setbit(1, 1);
    Node* rrrChild = new Node(LEAF_VAL, rrrMaj);
    if (!rrrChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed A3");
    }

    // level 4
    Maj* rrllMaj = new Maj(*rrlMaj);
    rrllMaj->setbit(2, 0);
    Node* rrllChild = new Node(-LEAF_VAL, rrllMaj);
    if (!rrllChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed A4");
    }

    Maj* rrlrMaj = new Maj(*rrlMaj);
    rrlrMaj->setbit(2, 1);
    Node* rrlrChild = new Node(LEAF_VAL, rrlrMaj);
    if (!rrlrChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed A5");
    }

    // connections

    // level 0
    aRoot->setLeft(lChild); aRoot->setRight(rChild);

    // level 1
    rChild->setLeft(rlChild); rChild->setRight(rrChild);

    // level 2
    rrChild->setLeft(rrlChild); rrChild->setRight(rrrChild);

    // level 3
    rrlChild->setLeft(rrllChild); rrlChild->setRight(rrlrChild);

    return aRoot;
}

Node* Tree::privMNSSTX_B(Maj* branchMaj) {
    // create nodes

    // level 0
    Node* bRoot = new Node(1, branchMaj);

    // level 1
    Maj* lMaj = new Maj(*branchMaj);
    lMaj->setbit(1, 0);
    Node* lChild = new Node(2, lMaj);

    Maj* rMaj = new Maj(*branchMaj);
    rMaj->setbit(1, 1);
    Node* rChild = new Node(LEAF_VAL, rMaj);
    if (!rChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed B1");
    }

    // level 2
    Maj* llMaj = new Maj(*lMaj);
    llMaj->setbit(2, 0);
    Node* llChild = new Node(4, llMaj);

    Maj* lrMaj = new Maj(*lMaj);
    lrMaj->setbit(2, 1);
    Node* lrChild = new Node(LEAF_VAL, lrMaj);
    if (!lrChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed B2");
    }

    // level 3
    Maj* lllMaj = new Maj(*llMaj);
    lllMaj->setbit(4, 0);
    Node* lllChild = new Node(-LEAF_VAL, lllMaj);
    if (!lllChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed B3");
    }

    Maj* llrMaj = new Maj(*llMaj);
    llrMaj->setbit(4, 1);
    Node* llrChild = new Node(5, llrMaj);

    // level 4
    Maj* llrlMaj = new Maj(*llrMaj);
    llrlMaj->setbit(5, 0);
    Node* llrlChild = new Node(-LEAF_VAL, llrlMaj);
    if (!llrlChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed B4");
    }

    Maj* llrrMaj = new Maj(*llrMaj);
    llrrMaj->setbit(5, 1);
    Node* llrrChild = new Node(LEAF_VAL, llrrMaj);
    if (!llrrChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed B5");
    }

    // connections

    // level 0
    bRoot->setLeft(lChild); bRoot->setRight(rChild);

    // level 1
    lChild->setLeft(llChild); lChild->setRight(lrChild);

    // level 2
    llChild->setLeft(lllChild); llChild->setRight(llrChild);

    // level 3
    llrChild->setLeft(llrlChild); llrChild->setRight(llrrChild);

    return bRoot;
}

Node* Tree::privMNSSTX_C(Maj* branchMaj) {
    // create nodes

    // level 0
    Node* cRoot = new Node(4, branchMaj);

    // level 1
    Maj* lMaj = new Maj(*branchMaj);
    lMaj->setbit(4, 0);
    Node* lChild = new Node(5, lMaj);

    Maj* rMaj = new Maj(*branchMaj);
    rMaj->setbit(4, 1);
    Node* rChild = new Node(LEAF_VAL, rMaj);
    if (!rChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed C1");
    }

    // level 2
    Maj* llMaj = new Maj(*lMaj);
    llMaj->setbit(5, 0);
    Node* llChild = new Node(6, llMaj);

    Maj* lrMaj = new Maj(*lMaj);
    lrMaj->setbit(5, 1);
    Node* lrChild = new Node(LEAF_VAL, lrMaj);
    if (!lrChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed C2");
    }

    // level 3
    Maj* lllMaj = new Maj(*llMaj);
    lllMaj->setbit(6, 0);
    Node* lllChild = new Node(7, lllMaj);

    Maj* llrMaj = new Maj(*llMaj);
    llrMaj->setbit(6, 1);
    Node* llrChild = new Node(7, llrMaj);

    // level 4
    Maj* llllMaj = new Maj(*lllMaj);
    llllMaj->setbit(7, 0);
    Node* llllChild = new Node(-LEAF_VAL, llllMaj);
    if (!llllChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed C3");
    }

    Maj* lllrMaj = new Maj(*lllMaj);
    lllrMaj->setbit(7, 1);
    Node* lllrChild = new Node(8, lllrMaj);

    Maj* llrlMaj = new Maj(*llrMaj);
    llrlMaj->setbit(7, 0);
    Node* llrlChild = new Node(8, llrlMaj);

    Maj* llrrMaj = new Maj(*llrMaj);
    llrrMaj->setbit(7, 1);
    Node* llrrChild = new Node(LEAF_VAL, llrrMaj);
    if (!llrrChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed C4");
    }

    // level 5
    Maj* lllrlMaj = new Maj(*lllrMaj);
    lllrlMaj->setbit(8, 0);
    Node* lllrlChild = new Node(-LEAF_VAL, lllrlMaj);
    if (!lllrlChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed C5");
    }

    Maj* lllrrMaj = new Maj(*lllrMaj);
    lllrrMaj->setbit(8, 1);
    Node* lllrrChild = new Node(LEAF_VAL, lllrrMaj);
    if (!lllrrChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed C6");
    }

    Maj* llrllMaj = new Maj(*llrlMaj);
    llrllMaj->setbit(8, 0);
    Node* llrllChild = new Node(-LEAF_VAL, llrllMaj);
    if (!llrllChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed C7");
    }

    Maj* llrlrMaj = new Maj(*llrlMaj);
    llrlrMaj->setbit(8, 1);
    Node* llrlrChild = new Node(LEAF_VAL, llrlrMaj);
    if (!llrlrChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed C8");
    }

    // connections
    
    // level 0
    cRoot->setLeft(lChild); cRoot->setRight(rChild);

    // level 1
    lChild->setLeft(llChild); lChild->setRight(lrChild);

    // level 2
    llChild->setLeft(lllChild); llChild->setRight(llrChild);

    // level 3
    lllChild->setLeft(llllChild); lllChild->setRight(lllrChild);
    llrChild->setLeft(llrlChild); llrChild->setRight(llrrChild);

    // level 4
    lllrChild->setLeft(lllrlChild); lllrChild->setRight(lllrrChild);
    llrlChild->setLeft(llrllChild); llrlChild->setRight(llrlrChild);

    return cRoot;
}

Node* Tree::privMNSSTX_D(Maj* branchMaj) {
    // create nodes

    // level 0
    Node* dRoot = new Node(6, branchMaj);

    // level 1
    Maj* lMaj = new Maj(*branchMaj);
    lMaj->setbit(6, 0);
    Node* lChild = new Node(7, lMaj);

    Maj* rMaj = new Maj(*branchMaj);
    rMaj->setbit(6, 1);
    Node* rChild = new Node(7, rMaj);

    // level 2
    Maj* llMaj = new Maj(*lMaj);
    llMaj->setbit(7, 0);
    Node* llChild = new Node(-LEAF_VAL, llMaj);
    if (!llChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed D1");
    }

    Maj* lrMaj = new Maj(*lMaj);
    lrMaj->setbit(7, 1);
    Node* lrChild = new Node(8, lrMaj);

    Maj* rlMaj = new Maj(*rMaj);
    rlMaj->setbit(7, 0);
    Node* rlChild = new Node(8, rlMaj);

    Maj* rrMaj = new Maj(*rMaj);
    rrMaj->setbit(7, 1);
    Node* rrChild = new Node(4, rrMaj);

    // level 3
    Maj* lrlMaj = new Maj(*lrMaj);
    lrlMaj->setbit(8, 0);
    Node* lrlChild = new Node(-LEAF_VAL, lrlMaj);
    if (!lrlChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed D2");
    }

    Maj* lrrMaj = new Maj(*lrMaj);
    lrrMaj->setbit(8, 1);
    Node* lrrChild = new Node(4, lrrMaj);

    Maj* rllMaj = new Maj(*rlMaj);
    rllMaj->setbit(8, 0);
    Node* rllChild = new Node(-LEAF_VAL, rllMaj);
    if (!rllChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed D3");
    }

    Maj* rlrMaj = new Maj(*rlMaj);
    rlrMaj->setbit(8, 1);
    Node* rlrChild = new Node(4, rlrMaj);

    Maj* rrlMaj = new Maj(*rrMaj);
    rrlMaj->setbit(4, 0);
    Node* rrlChild = new Node(5, rrlMaj);

    Maj* rrrMaj = new Maj(*rrMaj);
    rrrMaj->setbit(4, 1);
    Node* rrrChild = new Node(LEAF_VAL, rrrMaj);
    if (!rrrChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed D4");
    }

    // level 4
    Maj* lrrlMaj = new Maj(*lrrMaj);
    lrrlMaj->setbit(4, 0);
    Node* lrrlChild = new Node(5, lrrlMaj);

    Maj* lrrrMaj = new Maj(*lrrMaj);
    lrrrMaj->setbit(4, 1);
    Node* lrrrChild = new Node(LEAF_VAL, lrrrMaj);
    if (!lrrrChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed D5");
    }

    Maj* rlrlMaj = new Maj(*rlrMaj);
    rlrlMaj->setbit(4, 0);
    Node* rlrlChild = new Node(5, rlrlMaj);

    Maj* rlrrMaj = new Maj(*rlrMaj);
    rlrrMaj->setbit(4, 1);
    Node* rlrrChild = new Node(LEAF_VAL, rlrrMaj);
    if (!rlrrChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed D6");
    }

    Maj* rrllMaj = new Maj(*rrlMaj);
    rrllMaj->setbit(5, 0);
    Node* rrllChild = new Node(-LEAF_VAL, rrllMaj);
    if (!rrllChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed D7");
    }

    Maj* rrlrMaj = new Maj(*rrlMaj);
    rrlrMaj->setbit(5, 1);
    Node* rrlrChild = new Node(LEAF_VAL, rrlrMaj);
    if (!rrlrChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed D8");
    }

    // level 5
    Maj* lrrllMaj = new Maj(*lrrlMaj);
    lrrllMaj->setbit(5, 0);
    Node* lrrllChild = new Node(-LEAF_VAL, lrrllMaj);
    if (!lrrllChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed D9");
    }

    Maj* lrrlrMaj = new Maj(*lrrlMaj);
    lrrlrMaj->setbit(5, 1);
    Node* lrrlrChild = new Node(LEAF_VAL, lrrlrMaj);
    if (!lrrlrChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed D10");
    }

    Maj* rlrllMaj = new Maj(*rlrlMaj);
    rlrllMaj->setbit(5, 0);
    Node* rlrllChild = new Node(-LEAF_VAL, rlrllMaj);
    if (!rlrllChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed D11");
    }

    Maj* rlrlrMaj = new Maj(*rlrlMaj);
    rlrlrMaj->setbit(5, 1);
    Node* rlrlrChild = new Node(LEAF_VAL, rlrlrMaj);
    if (!rlrlrChild->majIsComputed()) {
        throw std::runtime_error("ERROR: maj not computed D12");
    }

    // connections

    // level 0
    dRoot->setLeft(lChild); dRoot->setRight(rChild);

    // level 1
    lChild->setLeft(llChild); lChild->setRight(lrChild);
    rChild->setLeft(rlChild); rChild->setRight(rrChild);

    // level 2
    lrChild->setLeft(lrlChild); lrChild->setRight(lrrChild);
    rlChild->setLeft(rllChild); rlChild->setRight(rlrChild);
    rrChild->setLeft(rrlChild); rrChild->setRight(rrrChild);

    // level 3
    lrrChild->setLeft(lrrlChild); lrrChild->setRight(lrrrChild);
    rlrChild->setLeft(rlrlChild); rlrChild->setRight(rlrrChild);
    rrlChild->setLeft(rrllChild); rrlChild->setRight(rrlrChild);

    // level 4
    lrrlChild->setLeft(lrrllChild); lrrlChild->setRight(lrrlrChild);
    rlrlChild->setLeft(rlrllChild); rlrlChild->setRight(rlrlrChild);

    return dRoot;
}

void Tree::buildComplete() {
    this->treeType = "Complete";
    Maj* rootMaj = new Maj();
    for (int i = 0; i < (VARS / 3); i++) {
        rootMaj->setbit(i, 1);
    }
    this->root = new Node((VARS / 3), rootMaj);

    Maj* lMaj = new Maj(*rootMaj);
    lMaj->setbit((VARS / 3), 0);
    this->root->setLeft(privBuildRandomSubtree(lMaj));

    Maj* rMaj = new Maj(*rootMaj);
    rMaj->setbit((VARS / 3), 1);
    this->root->setRight(privBuildRandomSubtree(rMaj));
}