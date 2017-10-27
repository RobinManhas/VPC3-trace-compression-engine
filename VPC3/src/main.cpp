
#include "VPC3.h"


using namespace std;



int main() {
    VPC3 vpc3;

    //Example one
    //config file location
    string cfg = "/Users/shweta/Documents/Masters@StonyBrook/Fall2017/OS-CSE506/VPC3/src/testconfig2.txt";
    //input file
    FILE *file = fopen("/Users/shweta/Documents/Masters@StonyBrook/Fall2017/OS-CSE506/VPC3/cmake-build-debug/Test2.txt","r");
    // location for output streams
    string output = "/Users/shweta/Documents/Masters@StonyBrook/Fall2017/OS-CSE506/VPC3/cmake-build-debug";
    if(!file) {
        cout << "Cannot open input file.\n";
        return 0;
    }

    // to compress
    vpc3.compress(file, cfg, output);
    fclose(file);

   // to decompress
    //output file
    char * input = "/Users/shweta/Documents/Masters@StonyBrook/Fall2017/OS-CSE506/VPC3/cmake-build-debug/output.txt";
    vpc3.decompress(output, cfg, input);


    return 0;
}
