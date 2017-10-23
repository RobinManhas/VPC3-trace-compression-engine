
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

    string cfg = "/Users/harsh/Documents/StonyBrook/FIrstSem/OS/Homework/OS_HW/VPC3/src/testconfig.txt";

    FILE *file = fopen("/Users/harsh/Documents/StonyBrook/FIrstSem/OS/Homework/OS_HW/VPC3/cmake-build-debug/test.txt","r");
    if(!file) {
        cout << "Cannot open input file.\n";
        return 0;
    }


    string output = "/Users/harsh/Documents/StonyBrook/FIrstSem/OS/Homework/OS_HW/VPC3/cmake-build-debug";

    //COnfig file path location
    //INput file location
    //Output file destination
    VPC3 vpc3;
    vpc3.encode(file,cfg,output);
    fclose(file);

    cout<<"ENcoding done"<<endl;
    char * input = "/Users/harsh/Documents/StonyBrook/FIrstSem/OS/Homework/OS_HW/VPC3/cmake-build-debug/output.txt";
    vpc3.decode(output,cfg,input);

    return 0;
}
