#include <iostream>
#include <fstream>
#include "VPC3.h"
#include "InputConfigReader.h"
#include "zlib.h"

using namespace std;
typedef unsigned char BYTE;
unsigned long file_size(char *filename)
{
    FILE *pFile = fopen(filename, "rb");
    fseek (pFile, 0, SEEK_END);
    unsigned long size = ftell(pFile);
    fclose (pFile);
    return size;
}

int decompress_one_file(char *infilename, char *outfilename)
{
    gzFile infile = gzopen(infilename, "rb");
    FILE *outfile = fopen(outfilename, "wb");
    if (!infile || !outfile) return -1;

    char buffer[128];
    int num_read = 0;
    while ((num_read = gzread(infile, buffer, sizeof(buffer))) > 0) {
        fwrite(buffer, 1, num_read, outfile);
    }

    gzclose(infile);
    fclose(outfile);
    return 0;
}

int compress_one_file(char *infilename, char *outfilename)
{
    FILE *infile = fopen(infilename, "rb");
    gzFile outfile = gzopen(outfilename, "wb");
    if (!infile || !outfile) return -1;

    char inbuffer[128];
    int num_read = 0;
    unsigned long total_read = 0, total_wrote = 0;
    while ((num_read = fread(inbuffer, 1, sizeof(inbuffer), infile)) > 0) {
        total_read += num_read;
        gzwrite(outfile, inbuffer, num_read);
    }
    fclose(infile);
    gzclose(outfile);

    printf("Read %ld bytes, Wrote %ld bytes, Compression factor %4.2f%%\n",
           total_read, file_size(outfilename),
           (1.0-file_size(outfilename)*1.0/total_read)*100.0);

    return 0;
}


int main() {
    //robin:
    char orig[150] = "/Users/shweta/Documents/Masters@StonyBrook/Fall2017/CSE506-OS/VPC3/src/testconfig.txt";
    char orig2[150] = "/Users/shweta/Documents/Masters@StonyBrook/Fall2017/CSE506-OS/VPC3/src/testresconfig.txt";
    char zipped[150] = "/Users/shweta/Documents/Masters@StonyBrook/Fall2017/CSE506-OS/VPC3/src/testconfig.zlib";


    TraceConfig* cfg = parseFile("/Users/shweta/Documents/Masters@StonyBrook/Fall2017/CSE506-OS/VPC3/src/testconfig.txt");

    cout<<"end, ID: "<< cfg->getFields()[0]->iFieldLen; // just for test, use this by getting vector appropriately

    //compress_one_file(orig,zipped);
    //decompress_one_file(zipped,orig2);



    ifstream fstr("/Users/shweta/Documents/Masters@StonyBrook/Fall2017/CSE506-OS/VPC3/cmake-build-debug/test.trace",std::ios::binary);
    if(!fstr) {
        cout << "Cannot open input file.\n";
        return 0;
    }

    VPC3 vpc3;


    vpc3.encode(fstr,cfg);

    int noOfFields = cfg->getFields().size();
    ifstream streams[2*noOfFields];
    for(int i = 0; i< 2*noOfFields; i++) {
        string suffix = "stream";
        suffix.append(1,i+'0');
        streams[i].open(suffix);
    }
    vpc3.decode(streams,cfg);

//    while(!fstr.eof()) {
//        buffer = new char[4];
//        //fstr.read(reinterpret_cast<char*>(&myint), sizeof(int));
//        fstr.read(buffer,4);
//        cout<<"loop: "<< buffer<<endl;
//    }
    //fstr.read(reinterpret_cast<char*>(&myint), sizeof(int));
    //vpc3.execute();
    cout<<"end";
    return 0;
}
