
#include "src/VPC3.h"


using namespace std;



int main() {
    VPC3 vpc3;

    //Example one
    //config file location
    string cfg = "../testconfig.txt";
    //input file
    FILE *file = fopen("../Test2.txt","r");
    // location for output streams
    string output = "../output";
    if(!file) {
        cout << "Cannot open input file.\n";
        return 0;
    }

    // to compress
    vpc3.compress(file, cfg, output);
    fclose(file);

   // to decompress
    //output file
    char * input = "../output/decompressed.txt";
    vpc3.decompress(output, cfg, input);


    return 0;
}
