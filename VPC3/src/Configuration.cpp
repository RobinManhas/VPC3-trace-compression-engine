//
// Created by robin manhas on 10/17/17.
//

#include "Configuration.h"
int TraceConfig::setHeader(int h){
    iHeaderLen = h;
    return 0;
}


int TraceConfig::addField(FieldInfo* info){
    fields.push_back(info);
    return 0;
}


int TraceConfig::setID(string ID){
    iID = ID;
    return 0;
}
int TraceConfig::setCompressor(string comp){
    compressor = comp;
    return 0;
}

int TraceConfig::setDecompressor(string decomp){
    decompressor = decomp;
    return 0;
}

int TraceConfig::getHeader(){
    return iHeaderLen;
}


vector<FieldInfo*> TraceConfig::getFields(){
    return fields;
}

string TraceConfig::getID(){
    return iID;
}

string TraceConfig::getCompressor(){
    return compressor;
}
string TraceConfig::getDecompressor(){
    return decompressor;
}


int TraceConfig::ref(){
    iRefCount+=1;
    return 0;
}

int TraceConfig::unref(){ // RM: banker
    iRefCount-=1;
    if(iRefCount == 0){
        //cout << "Refcount for TraceConfig became 0, deleting input" << endl;
        vector<FieldInfo*>::iterator it;
        for(it = fields.begin(); it != fields.end(); it++){
            if(*it){
                delete(*it);
            }
        }

        delete this;
    }
    return 0;
}
