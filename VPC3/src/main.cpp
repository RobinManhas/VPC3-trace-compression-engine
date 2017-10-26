
#include "VPC3.h"


using namespace std;



int main() {

    string cfg = "/Users/shweta/Documents/Masters@StonyBrook/Fall2017/OS-CSE506/VPC3/src/testconfig.txt";

    FILE *file = fopen("/Users/shweta/Documents/Masters@StonyBrook/Fall2017/OS-CSE506/VPC3/cmake-build-debug/Test2.txt","r");
    if(!file) {
        cout << "Cannot open input file.\n";
        return 0;
    }


    string output = "/Users/shweta/Documents/Masters@StonyBrook/Fall2017/OS-CSE506/VPC3/cmake-build-debug";

    //COnfig file path location
    //INput file :FILE*
    //Output file destination
    VPC3 vpc3;
    vpc3.encode(file,cfg,output);
    fclose(file);

    cout<<"ENcoding done"<<endl;
    char * input = "/Users/shweta/Documents/Masters@StonyBrook/Fall2017/OS-CSE506/VPC3/cmake-build-debug/output.txt";
    vpc3.decode(output,cfg,input);

    return 0;
}
