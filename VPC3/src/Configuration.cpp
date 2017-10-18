//
// Created by robin manhas on 10/17/17.
//

#include "Configuration.h"
int TraceConfig::setHeader(int h){
    iHeaderLen = h;
    return 0;
}


int TraceConfig::addPCField(FieldInfo* info){
    vPCField.push_back(info);
    return 0;
}

int TraceConfig::addEDField(FieldInfo* info){
    vEDField.push_back(info);
    return 0;
}

int TraceConfig::setID(string ID){
    iID = ID;
    return 0;
}

int TraceConfig::getHeader(){
    return iHeaderLen;
}


vector<FieldInfo*> TraceConfig::getPCField(){
    return vPCField;
}



vector<FieldInfo*> TraceConfig::getEDField(){
    return vEDField;
}

string TraceConfig::getID(){
    return iID;
}

int TraceConfig::ref(){
    iRefCount+=1;
    return 0;
}

int TraceConfig::unref(){ // RM: banker
    iRefCount-=1;
    if(iRefCount == 0){
        cout << "Refcount for TraceConfig became 0, deleting input" << endl;
        vector<FieldInfo*>::iterator it;
        for(it = vPCField.begin(); it != vPCField.end(); it++){
            if(*it){
                delete(*it);
            }
        }
        for(it = vEDField.begin(); it != vEDField.end(); it++){
            if(*it){
                delete(*it);
            }
        }
        delete this;
    }
    return 0;
}
