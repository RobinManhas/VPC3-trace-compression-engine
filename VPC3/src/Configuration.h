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
};

class TraceConfig{
private:
    int iRefCount;
    int iHeaderLen;
    vector<FieldInfo*> fields;
    string iID;
public:
    int ref();
    int unref();
    int setHeader(int h);
    int addField(FieldInfo* info);
    int setID(string ID);

    int getHeader();
    vector<FieldInfo*> getFields();
    string getID();
};
#endif //VPC3_CONFIGURATION_H
