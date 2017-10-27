//
// Created by robin manhas on 10/17/17.
//

#include "InputConfigReader.h"
#include "VPC3.h"

int convertToInt(string data){ // RM: In case atoi or c_str not supported, write implementation here
    return atoi(data.c_str());
}

TraceConfig* parseFile(string fpath){
    TraceConfig *config = new TraceConfig();
    int predictorFound = 0, L1found = 0,IDfound = 0;
    ifstream file(fpath);
    if(!file) {
        cout << "Cannot open input file.\n";
        return NULL;
    }
    string lbuf;

    while (file && getline(file, lbuf)) {
        if (lbuf.length() == 0){
            continue;
        }
        //cout << "Line: " <<lbuf <<endl;

        // parse bit header
        if(lbuf.find("Header") != string::npos){
            int val = convertToInt(lbuf.substr(0,lbuf.find("-")));
            config->setHeader(val);
            //cout<<"Bit header found: "<<val<<endl;
        }

        // parse bit field
        else if(lbuf.find("Bit Field") != string::npos){
            int val = convertToInt(lbuf.substr(0,lbuf.find("-")));
            predictorFound = 0;
            int totalPredictors=0;
            FieldInfo* f = new FieldInfo();

            // getting field ID
            int first = lbuf.find("Field");
            first+= 6;
            int last = lbuf.find("=",0);
            if(first >=0 && last >= 0) {
                int fieldID = convertToInt(lbuf.substr(first, last - first));
                //cout << "got FieldID: "<<fieldID<<endl;
                f->iFieldID = fieldID;
            }

            // getting L1
            first = lbuf.find("L1 = ");
            last = lbuf.find(",",0);
            if(first >=0 && last >= 0){
                string strNew = lbuf.substr (first,last-first);
                //cout << "got L1: "<<strNew<<endl;
                f->iL1 = convertToInt(strNew);
                if(L1found == 0){
                    L1found = f->iL1;
                }
            }
            else{
                //cout << "L1 values NOT FOUND"<<endl;
                // if l1 not found and we have a previous l1 value, l1 defaults to 32768, else l1 defaults to 1
                if(L1found == 0)
                    f->iL1 = 1;
                else
                    f->iL1 = 32768;
            }

            // getting L2
            first = lbuf.find("L2 = ");
            first+=5;
            last = lbuf.find(":",0);
            if(first >=0 && last >= 0){
            string strNew = lbuf.substr (first,last-first);
            //cout << "got L2: "<<strNew<<endl;
            f->iL2 = convertToInt(strNew);
            }
            else{
                //cout << "L2 values NOT FOUND, default 65536"<<endl;
                f->iL2 = 65536;
            }

            // getting list of predictors
            first = lbuf.find(":",1);
            first+=1;
            last = lbuf.find("}",0);
            if(first >=0 && last >= 0){
                string predictorStr = lbuf.substr (first,(last+1)-first);
                //cout << "predictor str"<<predictorStr<<endl;
                int count = 0;first = 0;
                int len = predictorStr.length();

                // find all csv of predictors
                do{
                    last = predictorStr.find(",",0);
                    if(last >= 0 && last <= len){
                        string strNew = predictorStr.substr (first,last-first);
                        //cout << "Predictor: "<<strNew<<endl;

                        // split predictor name and count
                        int namestart = 0;
                        int namend = strNew.find("[",0);
                        string name = strNew.substr (namestart,namend-namestart);
                        namestart = namend+1;
                        namend = strNew.find("]",0);
                        int value = convertToInt(strNew.substr (namestart,namend-namestart));

                        f->mPredictorMap.insert(make_pair(name, value));
                        totalPredictors += value;
                        //cout <<"Predictor added to map,name: "<<name<<" ,val "<<value<<endl;
                        predictorStr.erase(0,strNew.length()+1);
                        predictorFound = 1;
                        count++;
                    }
                    else{
                        break;
                    }

                }
                while(count < 20); // RM: Just a boundary check, ideally loop must break before.

                // for final predictor without comma
                //cout << "predictor now: "<<predictorStr<<endl;
                last = predictorStr.find("}",0);
                first = 0;
                if(last >= 0){
                    string strNew = predictorStr.substr (first,last-first);
                    //cout << "finally found: "<<strNew<<endl;
                    // split predictor name and count
                    int namestart = 0;
                    int namend = strNew.find("[",0);
                    string name = strNew.substr (namestart,namend-namestart);
                    namestart = namend+1;
                    namend = strNew.find("]",0);
                    int value = convertToInt(strNew.substr (namestart,namend-namestart));

                    f->mPredictorMap.insert(make_pair(name, value));
                    totalPredictors += value;
                    //cout <<"Predictor added to map,name: "<<name<<" ,val "<<value<<endl;
                    predictorStr.erase(0,strNew.length()+1);
                    predictorFound = 1;
                }
            }

            if(!predictorFound){
                f->mPredictorMap.insert(make_pair(DFCM3, 2));
                f->mPredictorMap.insert(make_pair(FCM3, 2));
                f->mPredictorMap.insert(make_pair(LV, 4));
            }

            f->iFieldLen = val/8;
            f->totalPredictors=totalPredictors;
            config->addField(f);


        }

        else if(lbuf.find("ID =") != string::npos){
            int first = lbuf.find("=",1);
            first+=2;
            int last = lbuf.find(";",0);
            if(first >=0 && last >= 0) {
                string sID = lbuf.substr(first, last - first);
                config->setID(sID);
                IDfound = 1;
            }
        }else if(lbuf.find("Compressor = ") != string::npos){
            int first = lbuf.find("'",1);
            first+=1;
            int last = lbuf.find("';",0);
            if(first >=0 && last >= 0) {
                string comp = lbuf.substr(first, last - first);
                config->setCompressor(comp);
                IDfound = 1;
            }
        }else if(lbuf.find("Decompressor = ") != string::npos){
            int first = lbuf.find("'",1);
            first+=1;
            int last = lbuf.find("';",0);
            if(first >=0 && last >= 0) {
                string decomp = lbuf.substr(first, last - first);
                config->setDecompressor(decomp);
                IDfound = 1;
            }
        }
    }

    if(!IDfound){
        config->setID("Field 1"); // ideal case we must traverse all fields to find the one with L1 as 1
    }

    return config;
}