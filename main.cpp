//simple driver program to run the functions
#include <iostream>
#include "Character.h"
#include "Link.h"
using namespace std;


int main (){ 
  Link link1;
  Character * character1= &link1;
  character1->runfunc(); //runs runfunc for link1 object

  return 0;
}
