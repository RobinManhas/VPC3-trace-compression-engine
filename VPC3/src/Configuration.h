//
// Created by robin manhas on 10/17/17.
//

#ifndef VPC3_CONFIGURATION_H
#define VPC3_CONFIGURATION_H

#include <iostream>
#include <map>
#include <vector>
#define MAX_FIELDS_SUPPORTED 10 // There can be at most 10 TraceSpec currently supported

using namespace std;

class FieldInfo{
public:
    int iFieldLen;
    int iFieldID;
    int iL1;
    int iL2;
    map <string,int> mPredictorMap;
    int totalPredictors;

};

class TraceConfig{
private:
    int iRefCount;
    int iHeaderLen;
    vector<FieldInfo*> fields;
    string iID;
    string compressor="bzip2 -c -z -9";
    string decompressor="bzip2 -c -d";
public:
    int ref();
    int unref();
    int setHeader(int h);
    int addField(FieldInfo* info);
    int setID(string ID);
    int setCompressor(string comp);
    int setDecompressor(string decomp);

    int getHeader();
    vector<FieldInfo*> getFields();
    string getID();
    string getCompressor();
    string getDecompressor();
};
#endif //VPC3_CONFIGURATION_H
