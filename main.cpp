#include <iostream>
#include "Character.h"
#include "Link.h"
using namespace std;

int main (){ // ( int argc, char* args[] ) {
  Link link1;
  Character * character1= &link1;
  // character1->runfunc();
  character1->runfunc();
  return 0;
}
