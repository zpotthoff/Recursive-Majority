#include <iostream>
#include "maj.h"

#define CHOICES 6

using namespace std;

void printMenu() {
  cout << "welcome to the 3-MAJ calculator!" << endl;
  cout << "what would you like to do? " << endl;
  cout << "  (0) query a bit" << endl;
  cout << "  (1) check to see if we have computed 3-MAJ" << endl;
  cout << "  (2) unqery a bit" << endl;
  cout << "  (3) which bits have been queried?" << endl;
  cout << "  (4) inspect state" << endl;
  cout << "  (5) quit" << endl;
}

int getChoice(int low, int high) {
  int choice;

  cin >> choice;
  while(choice < low || choice > high) {
    cout << "not a valid answer.  Number must be between "
	 << low << " and " << high << ". Please try again" << endl;
    
    cin >> choice;
  }
  return choice;
}

int main() {
  Maj maj;
  int choice;

  while(true) {
    printMenu();
    choice = getChoice(0,CHOICES+1);

    if(choice == 0) {
      cout << "which bit to query? ";
      int k;
      k = getChoice(0,VARS);
      
      cout << "what is its value? ";
      int val;
      val = getChoice(0,2);
      
      maj.setbit(k,val);
      if(maj.isComputed()) {
	cout << "we've computed recursive majority!" << endl;
	cout << "it's value is: " << maj.getValue() << endl;
      }
    } else if (choice == 1) {
      if(maj.isComputed()) {
	cout << "we've computed recursive majority!" << endl;
	cout << "it's value is: " << maj.getValue() << endl;
      } else {
	cout << "we have not yet computed recursive majority" << endl;
      }
      
    } else if (choice == 2) {
      cout << "oops, *un*-querying a bit isn't implemented yet" << endl;
    } else if (choice == 3) {
      cout << "bits queried: " << maj.whichbits() << endl;
    } else if (choice == 4) {
      maj.inspect();
    }
    else { // quit
      cout << " goodbye!" << endl;
      return 1;
    }
  }
}
