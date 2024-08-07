#include <iostream>
#include <sstream>
#include "maj.h"

using namespace std;

Maj::Maj() {
  //verify our internal nodes are correct
  for (int i = BSSIZE-VARS; i < BSSIZE; i+=3) {
      checkParent(i);
  }
  // cout << "[" << this->computed << "]" << endl;
}

Maj::Maj(bitset<BSSIZE> computed, bitset<BSSIZE> values) {
  for (int i = BSSIZE-VARS; i < BSSIZE; i+=3) {
      checkParent(i);
  }
  this->computed = computed;
  this->values = values;
}

Maj::Maj(const Maj& m) {
  this->computed = m.computed;
  this->values = m.values;
}

string Maj::whichbits() {
  // gather which bits have been queried in a string
  stringstream toReturn;

  for(int i=BSSIZE-VARS; i<BSSIZE; i++) {
    if(this->computed[i]) {
      toReturn << i-(BSSIZE-VARS) <<" ";
    }
  }
  return toReturn.str();
}

void Maj::setbit(int k, int val) {
  // need to convert bit index to leaf index
  k += BSSIZE-VARS;
  
  if(this->computed[k]) {
    throw runtime_error("setbit called, but bit already set");
  }
  this->computed[k]=1;
  this->values[k] = val;
  checkParent(k);
}

void Maj::unsetbit(int k) {
  // need to convert bit index to leaf index
  k += BSSIZE-VARS;
  if(!this->computed[k]) {
    throw runtime_error("unsetbit called, but bit isn't currently set");
  }
  this->computed[k]=0;

  checkParent(k);
}

void Maj::checkParent(int k) {
  int parent = this->getParent(k);
  int a = this->getLeftChild(parent);
  int b = a+1;
  int c = a+2;

  if(check(a,b)) {
    this->computed[parent] = 1;
    this->values[parent] = this->values[a];
   
  } else if (check(a,c)) {
    this->computed[parent]=1;
    this->values[parent] = this->values[a];
  } else if (check(b,c)) {
    this->computed[parent]=1;
    this->values[parent] = this->values[b];
  } else { // can't compute this; make sure bit is unset
    this->computed[parent]=0;
  }
    
  if(!isRoot(parent)) {
    checkParent(parent);
  }
			     
}

bool Maj::check(int x, int y) {
  if(this->computed[x] && this->computed[y]) {
    if(this->values[x] == this->values[y]) {
      return true;
    }
  }
  return false;
}

bool Maj::isComputed() {
  return this->computed[0];
}

int Maj::getValue() {
  if(!this->isComputed()) {
    throw runtime_error("getValue called but value not yet computed");
  }
  return this->values[0];
}

bitset<BSSIZE> Maj::getComputed() {
  return this->computed;
}

bitset<BSSIZE> Maj::getBitValues() {
  return this->values;
}

bool Maj::isRoot(int k) {
  return k==0;
}

void Maj::inspect() {
  cout << "computed: [" << this->computed << "]" << endl;
  cout << "values: [" << this->values << "]" << endl;
}

int Maj::getParent(int k) {
  int parent = (k-1)/3;
  if(parent<0) {
    throw runtime_error("called getParent on index that has no parent");
  }
  return parent;
}

int Maj::getLeftChild(int k) {
  int child = 3*k+1;
  if(child >= BSSIZE) {
    throw runtime_error("called getLeftChild on left index");
  }
  return child;
}
