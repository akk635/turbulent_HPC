#include <iostream>
#include <fstream>
#include <assert.h>
#include "Definitions.h"
using namespace std;

int main ( int argc, char *argv[]){
ifstream f0, f1;
f0.open(argv[1]);
f1.open(argv[2]);
FLOAT v0, v1;
int count = 0;
while(!(f1.fail())){
f0 >> v0;
f1 >> v1;
count++;
cout << count << endl;
assert(v0 == v1);
}
f0.close();
f1.close();
return 0;
}
