#pragma once

#include <bitset>
#include "globals.h"

using std::bitset;
using std::string;

class Maj {
 public:
  Maj();
  Maj(bitset<BSSIZE> computed, bitset<BSSIZE> values);
  Maj(const Maj& m);
  void inspect();
  void setbit(int k,int val);
  void unsetbit(int k);
  bool isComputed();
  string whichbits();
  int getValue();
  bitset<BSSIZE> getComputed();
  bitset<BSSIZE> getBitValues();

 private:
  int numVars;
  bitset<BSSIZE> computed;
  bitset<BSSIZE> values;

  bool isRoot(int k);
  int getParent(int k);
  int getLeftChild(int k);
  void checkParent(int k);
  bool check(int x, int y);
};
