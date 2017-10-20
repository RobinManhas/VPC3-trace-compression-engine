//
// Created by Shweta Sahu on 10/3/17.
//

#include "VPC3.h"
#include "FCMPredictor.h"
#include "DFCMPredictor.h"
#include "LVPredictor.h"
#include "Predictor.h"
#include "FCMPredictor.cpp"
#include "DFCMPredictor.cpp"
#include "LVPredictor.cpp"
#include "main.h"
using namespace std;


/*
 *  * *********************************   PREDICTORS  START ******************************************************
 */



void VPC3::preparePredictors(TraceConfig* cfg) {
    int noOfFields = cfg->getFields().size();
    int size = 0;
    predictorsListofLists = new void**[noOfFields];
    for(int i = 0; i<noOfFields; i++) {
        size = cfg->getFields()[i]->iFieldLen / 8;
        switch (size) {
            case 4:
                predictorsListofLists[i] = (void **) assignPredictors<uint32_t>(cfg, i);
                break;
            case 8:
                predictorsListofLists[i] = (void **) assignPredictors<uint64>(cfg, i);
                break;
        }
    }
}

template <typename T>
Predictor<T> ** VPC3::assignPredictors(TraceConfig* cfg,int i) {
    int counter = 0;
    int countOfpredictor = 0;
    int id = 0;
    int noOfPredictors = cfg->getFields()[i]->totalPredictors;
    Predictor<T> **predictors= new Predictor<T>*[noOfPredictors];
    map<string, int>::iterator it;
    bool isFCMPredictorsPresent = false;
    map<string, int> fcmOrderMap;
    int totalFCMPredictors = 0;

    bool isDFCMPredictorsPresent = false;
    map<string, int> dfcmOrderMap;
    int totalDFCMPredictors = 0;

    for (it = cfg->getFields()[i]->mPredictorMap.begin(), counter = 0;
         it != cfg->getFields()[i]->mPredictorMap.end(); it++, counter++) {

        countOfpredictor = it->second;

        if (it->first == LV) {
            getLVPredictors<T>(countOfpredictor, predictors, id);
            id = id + countOfpredictor;

        } else if (it->first.substr(0,3) == "FCM") {
            isFCMPredictorsPresent = true;
            string order = it->first.substr(3,1);
            fcmOrderMap.insert(make_pair(order, it->second));
            totalFCMPredictors += it->second;

        }else if (it->first.substr(0,4) == "DFCM") {
            isDFCMPredictorsPresent = true;
            string order = it->first.substr(4,1);
            dfcmOrderMap.insert(make_pair(order, it->second));
            totalDFCMPredictors += it->second;
        }

    }
    if(isFCMPredictorsPresent){
        id = getFCMPredictors(predictors,id,cfg->getFields()[i]->iL2,fcmOrderMap);
    }
    if(isDFCMPredictorsPresent){
        id = getDFCMPredictors(predictors,id,cfg->getFields()[i]->iL2,dfcmOrderMap);
    }

    return predictors;
}


template <typename T>
int VPC3::getLVPredictors(int countOfpredictor, Predictor<T> *predictors[],int id){

    T *lvTable= new T[countOfpredictor];
    for (int i = 0; i < countOfpredictor; i++) {
        lvTable[i] = 0;
    }
    for(int z = 0; z< countOfpredictor; z++){
        LVPredictor<T> *l = new LVPredictor<T>();
        l->initialise(lvTable, countOfpredictor, id);
        predictors[id++]=l;

    }
    return id;
}

template <typename T>
int VPC3::getFCMPredictors(Predictor<T> *predictors[],int id, int const hashTableSize,map<string, int> fcmOrderMap){

    int  maxOrder = 3; // to be processed;
    char recent[] = {'a','b'};
    T *secondLevelTable[hashTableSize];

    for (T i = 0; i < hashTableSize; i++) {
        secondLevelTable[i] = new T[2];
        for (T j = 0; j < 2; j++) {
            secondLevelTable[i][j] = 0;
        }
    }

    int *firstLevelTable = new int[maxOrder];
    for (int i = 0; i < maxOrder; i++) {
        firstLevelTable[i] = 0;
    }
    map<string, int>::iterator it;
    for (it = fcmOrderMap.begin(); it != fcmOrderMap.end(); it++) {
        int countOfpredictor = it->second;
        for (int z = 0; z < countOfpredictor; z++) {
            FCMPredictor<T> *f = new FCMPredictor<T>();
            f->initialise(firstLevelTable, secondLevelTable, hashTableSize, maxOrder, stoi(it->first), recent[z], id);
            predictors[id++] = f;
        }
    }
    return id;

}

template <typename T>
int VPC3::getDFCMPredictors(Predictor<T> *predictors[],int id, int const hashTableSize,map<string, int> dfcmOrderMap){

    int  maxOrder = 3;
    char recent[] = {'a','b'};
    T *secondLevelTable[hashTableSize];

    for (T i = 0; i < hashTableSize; i++) {
        secondLevelTable[i] = new T[2];
        for (T j = 0; j < 2; j++) {
            secondLevelTable[i][j] = 0;
        }
    }

    int *firstLevelTable = new int[maxOrder+1];
    for (int i = 0; i <=  maxOrder; i++) {
        firstLevelTable[i] = 0;
    }
    map<string, int>::iterator it;
    for (it = dfcmOrderMap.begin(); it != dfcmOrderMap.end(); it++) {
        int countOfpredictor = it->second;
        for (int z = 0; z < countOfpredictor; z++) {
            DFCMPredictor<T> *d = new DFCMPredictor<T>();
            d->initialise(firstLevelTable, secondLevelTable, hashTableSize, maxOrder, stoi(it->first), recent[z], id);
            predictors[id++] = d;
        }
    }
    return id;

}

template<typename T>
void VPC3::updatePredictors(Predictor<T>* predictors[],T value,int noOfPredictors){
    //flag set
    FCMPredictor<T>::setUpdateFlag();
    for( int index=0; index < noOfPredictors; index++) {
        predictors[index]->update(value);
    }

}

template<typename T>
T VPC3::getValueFromPredictor(Predictor<T>* predictors[], int predictorId){
    return predictors[predictorId]->getPrediction();
}

template<typename T>
int VPC3::findCorrectPredictor(const T value,unsigned int noOfPredictors,Predictor<T>* predictors[]){
    int predictorId = noOfPredictors;
    int usageCount = -1;
    for( int index=0; index < noOfPredictors; index++){
        if((predictors[index]->getPrediction() == value) && (predictors[index]->getUsageCount() > usageCount)){
            predictorId = predictors[index]->getPredictorId();
            usageCount = predictors[index]->getUsageCount();
            predictors[index]->incrementUsageCount();
        }
        //Need to check for LVP
//        else {
//            predictors[index]->update(value);
//        }

    }
    //Added according to FCMP: update if no correct prediction
    if(predictorId==noOfPredictors){
        updatePredictors(predictors,value,noOfPredictors);
    }
    return predictorId;
}


/////////////////////////////////////// PREDICTORS END ////////////////////////////////////////////////

/*
 * *********************************   ENCODING  START ******************************************************
 */

void VPC3::encode(ifstream& fstr,TraceConfig* cfg){
    //predictors initialisation
    preparePredictors(cfg);

    int noOfFields = cfg->getFields().size();
    //initialising 2 streams for each field
    ofstream streams[2*noOfFields];
    for(int i = 0; i< 2*noOfFields; i++) {
        string suffix = "stream";
        suffix.append(1,i+'0');
        streams[i].open(suffix);
    }
    do {
        for (int i = 0; i < noOfFields; i++) {
            int size = cfg->getFields()[i]->iFieldLen/8;
            switch (size) {
                case 4:
                    encodePredictions<uint32_t>(fstr,cfg, i, size,streams);
                    break;
                case 8:
                    encodePredictions<uint64>(fstr,cfg, i, size,streams);
                    break;
                default:
                    cout << "invalid size"<<endl;
            }

        }
    }while(!fstr.eof());

    for(int i = 0; i< 2*noOfFields; i++) {
        streams[i].close();
    }

}

template <typename T>
T VPC3::encodePredictions(ifstream& fstr,TraceConfig* cfg, int i,int size,ofstream streams[]){

    T data = readBytes<T>(fstr,size);//check for last data

    int predictorId =    findCorrectPredictor<T>(data, cfg->getFields()[i]->totalPredictors,
                                                 (Predictor<T> **) predictorsListofLists[i]);
    //write in first stream
    if(predictorId > 9){
        char encodedId = predictorId+'0';
        streams[2*i]<<encodedId;
    }
    else
        streams[2*i]<<predictorId;



    if(predictorId == cfg->getFields()[i]->totalPredictors){
        //write in second stream
        char * tmp = convertToChar<T>(data,size);
        streams[2*i+1]<<tmp;
    }

    return data;
}

//////////////////////////////////////// ENCODING END ///////////////////////////////////////////////////

/*
 *  * *********************************   DECODING  START ******************************************************
 */

void VPC3::decode(ifstream streams[],TraceConfig* cfg){
    preparePredictors(cfg);

    ofstream file;
    file.open ("output.txt");
    file << "Output trace\n";
    int noOfFields = cfg->getFields().size();

    do {
        for (int i = 0; i < noOfFields; i++) {
            int size = cfg->getFields()[i]->iFieldLen/8;
            switch (size) {
                case 4:
                    decodePredictions<uint32_t>(streams,cfg,i,size,file);

                    break;
                case 8:
                    decodePredictions<uint64>(streams,cfg,i,size,file);
                    break;
                default:
                    cout << "invalid size"<<endl;
            }
        }
    }while(!streams[0].eof());

}

template <typename T>
T VPC3::decodePredictions(ifstream streams[],TraceConfig* cfg, int i,int size,ofstream& file) {
    T data;
    int predictorId = readBytes<char>(streams[2*i],1)-'0';
    if(predictorId>=0){
        if(predictorId == cfg->getFields()[i]->totalPredictors){
            data = readBytes<T>(streams[2*i+1],size);
            file<<convertToChar<T>(data,size);
            updatePredictors((Predictor<T> **) predictorsListofLists[i],data,cfg->getFields()[i]->totalPredictors);
        }
        else{
            //take value from predictor
            data = getValueFromPredictor<T>((Predictor<T> **)predictorsListofLists[i],predictorId);
            file<<convertToChar<T>(data,size);

        }
    }
    return data;
}

//////////////////////////////////////// ENCODING END ///////////////////////////////////////////////////


/*
 *  * *********************************   UTILS  START ******************************************************
 */


template<typename T>
//extra 0 in the end is getting read ## to be fixed
T VPC3::readBytes(ifstream& fstr,int noOfBytes){
    T data =0;
    T tmpdata = 0;
    if(!fstr || fstr.eof()){
        cout<<"file reference not found or file end is encountered "<<endl;
        return NULL;
    }
   fstr.read(reinterpret_cast<char*>(&data), noOfBytes);
    std::streamsize bytes = fstr.gcount();
    if(bytes==0){
        return NULL;
    }
   return data;

}

template<typename T>
char * VPC3::convertToChar(T value,int noOfBytes){
    char* stream = new char[noOfBytes+1];
    for(int i =0; i<noOfBytes; i++ ){
        stream[i] = (value)&0xFF;
        value = value >>8;
    }
    stream[noOfBytes]='\0';
    return stream;

}


template<typename T>
T VPC3::convertToT(const char* buffer, int noOfBytes){
    T data =0;
    for(int i =noOfBytes-1; i>=0; i-- ){
        data = (data << 8)+buffer[i];
    }
   return data;
}



