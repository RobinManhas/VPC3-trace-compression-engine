//
// Created by Shweta Sahu on 10/3/17.
//

#include "VPC3.h"
#include "FCMPredictor.h"
#include "LVPredictor.h"
#include "Predictor.h"
#include "FCMPredictor.cpp"
#include "LVPredictor.cpp"
//#include "Predictor.cpp"
#include "main.h"
using namespace std;

//typedef unsigned int uint32_t ;
typedef unsigned long uint64 ;

void VPC3::prepare() {
    cout << "inside prepare\n\n";


    //for size 4bytes
    unsigned int const hashTableSize = 131072; // have to use variable size
    uint32_t *secondLevelTable[hashTableSize];//contains most recent value that followed the input , second recent
    int maxOrder = 3;
    Predictor<uint32_t> *predictors1[4];

    //n defined as 2 in main.h
    for (uint32_t i = 0; i < hashTableSize; i++) {
        secondLevelTable[i] = new uint32_t[n];
        for (uint32_t j = 0; j < n; j++) {
            secondLevelTable[i][j] = 0;
        }
    }


    uint32_t *firstLevelTable = new uint32_t[maxOrder];
    for (int i = 0; i < maxOrder; i++) {
        firstLevelTable[i] = 0;
    }


    //firsttable, secondtable,hashtablesize,maxorder,order,recent,id
    FCMPredictor<uint32_t> *f1 = new FCMPredictor<uint32_t>();
    f1->initialise(firstLevelTable, secondLevelTable, hashTableSize, maxOrder, 1, 'a', 0);

    FCMPredictor<uint32_t> *f2 = new FCMPredictor<uint32_t>();
    f2->initialise(firstLevelTable, secondLevelTable, hashTableSize, maxOrder, 1, 'b', 1);

    FCMPredictor<uint32_t> *f3 = new FCMPredictor<uint32_t>();
    f3->initialise(firstLevelTable, secondLevelTable, hashTableSize, maxOrder, 3, 'a', 2);

    FCMPredictor<uint32_t> *f4 = new FCMPredictor<uint32_t>();
    f4->initialise(firstLevelTable, secondLevelTable, hashTableSize, maxOrder, 3, 'b', 3);


    predictors1[0] = f1;
    predictors1[1] = f2;
    predictors1[2] = f3;
    predictors1[3] = f4;


    unsigned int hashTableSize2 = 131072;
    uint64 *secondLevelTable2[hashTableSize2];//contains most recent value that followed the input , second recent
    for (int i = 0; i < hashTableSize2; i++) {
        secondLevelTable2[i] = new uint64[n];
        for (int j = 0; j < n; j++) {
            secondLevelTable[i][j] = 0;
        }
    }

    int maxOrder2 = 1;
    uint32_t *firstLevelTable2 = new uint32_t[maxOrder2];
    for (int i = 0; i < maxOrder2; i++) {
        firstLevelTable2[i] = 0;
    }

    int const noOfLVpredictors = 4;
    uint64 *lvTable = new uint64[noOfLVpredictors];
    for (int i = 0; i < noOfLVpredictors; i++) {
        lvTable[i] = 0;
    }

    LVPredictor<uint64> *l1 = new LVPredictor<uint64>();
    l1->initialise(lvTable, noOfLVpredictors, 0);
    LVPredictor<uint64> *l2 = new LVPredictor<uint64>();
    l2->initialise(lvTable, noOfLVpredictors, 1);
    LVPredictor<uint64> *l3 = new LVPredictor<uint64>();
    l3->initialise(lvTable, noOfLVpredictors, 2);
    LVPredictor<uint64> *l4 = new LVPredictor<uint64>();
    l4->initialise(lvTable, noOfLVpredictors, 3);

    FCMPredictor<uint64> *f21 = new FCMPredictor<uint64>();
    f21->initialise(firstLevelTable2, secondLevelTable2, hashTableSize2, maxOrder2, 1, 'a', 4);

    FCMPredictor<uint64> *f22 = new FCMPredictor<uint64>();
    f22->initialise(firstLevelTable2, secondLevelTable2, hashTableSize2, maxOrder2, 1, 'b', 5);
//    Predictor<uint64>* predictors2[]= {l1,l2,l3,l4,f21,f22};
    Predictor<uint64> *predictors2[6];

    predictors2[0] = l1;
    predictors2[1] = l2;
    predictors2[2] = l3;
    predictors2[3] = l4;
    predictors2[4] = f21;
    predictors2[5] = f22;


    predictorsListofLists[0] = (void **) predictors1;
    predictorsListofLists[1] = (void **) predictors2;


}
/*
 * takes two streams ;
 */
void VPC3::decodeStreams(ifstream streams[],TraceConfig* cfg){//string to be replaced by streams
    cout<<"inside decode"<<endl;
    int i = 0;
    int j = 0;


    ofstream file;
//    char buffer[8192];
//    int size = 0;
    file.open ("output.txt");
    file << "Output trace\n";
    int noOfFields = cfg->getFields().size();

    int predictorId;
    char* data;
    uint32_t data4;
    uint64 data8;
    do {
        for (int i = 0; i < noOfFields; i++) {
            int size = cfg->getFields()[i]->iFieldLen/8;
            switch (size) {
                case 4:
                    //cout<<"in 4 "<<endl;
                    //predictorId = stream1[i1++]- '0';
                    predictorId = readBytes<char>(streams[2*i],1)-'0';
                    //cout<<"predictorid: "<<predictorId<<endl;
                    if(predictorId>=0){
                        if(predictorId == totalpredictors[i]){
                            //read from stream2
                            //cout<<i2<<endl;
                            data4 = readBytes<uint32_t>(streams[2*i+1],size);
                            data = convertToChar<uint32_t>(data4,size);
//                            strcat(buffer,data);
//                            size+=sizes[i];
                            file<<data;
                            updatePredictors((Predictor<uint32_t> **) predictorsListofLists[i],data4,totalpredictors[i]);
                        }
                        else{
                            //take value from predictor
                            cout<<"4: take value from predictor id: "<<predictorId<<endl;
                            data4 = getValueFromPredictor<uint32_t>((Predictor<uint32_t> **)predictorsListofLists[i],predictorId);
                            cout<<"value from predictor: "<<data4<<endl;
                            file<<convertToChar<uint32_t>(data4,size);
//                            data=convertToChar<uint32_t>(data4,sizes[i]);
//                            strcat(buffer,data);
//                            size+=sizes[i];

                        }
                    }

                    break;
                case 8:
                    predictorId = readBytes<char>(streams[2*i],1)-'0';
                    if(predictorId>=0) {
                        if (predictorId == totalpredictors[i]) {
                            data8 = readBytes<uint64>(streams[2*i+1],size);
                            data = convertToChar<uint64>(data8,size);
//                            strcat(buffer,data);
//                            size+=sizes[i];
                            file<<data;
                            updatePredictors((Predictor<uint64> **) predictorsListofLists[i], data8,
                                             totalpredictors[i]);
                        } else {
                            //take value from predictor
                            data8 = getValueFromPredictor<uint64>((Predictor<uint64> **) predictorsListofLists[i],
                                                                  predictorId);
                            file << convertToChar<uint64>(data8, size);
//                            data=convertToChar<uint64>(data8, sizes[i]);
//                            strcat(buffer,data);
//                            size+=sizes[i];

                        }
                    }
                   break;
                default:
                    cout << "invalid size"<<endl;
            }

//            if(size==8192){
//                file.write(buffer,size);
//                size=0;
//            }
        }
    }while(!streams[0].eof());

//    if(size<8192)
//    file.write(buffer,size);
    //file.close();

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

void VPC3::decode(ifstream streams[],TraceConfig* cfg){
    prepare();

    decodeStreams(streams,cfg);
    //decodeStreams(idStream,dataStream);

}

void VPC3::encode(ifstream& fstr,TraceConfig* cfg){
    //predictors initialisation
    prepare();

    int noOfFields = cfg->getFields().size();
    //initialising 2 streams for each field
    ofstream streams[2*noOfFields];
    for(int i = 0; i< 2*noOfFields; i++) {
        string suffix = "stream";
        suffix.append(1,i+'0');
        streams[i].open(suffix);
    }

    int predictorId = -1;
    uint32_t data4;
    uint64 data8;
    char *tmp;
    do {
        for (int i = 0; i < noOfFields; i++) {
            int size = cfg->getFields()[i]->iFieldLen/8;
            switch (size) {
                /*
                 * size[i] read from file
                 * predictorsListofLists[i] list of predictors
                 */
                case 4:

                    data4 = readBytes<uint32_t>(fstr,size);
                    if(data4 == NULL)
                        break;
                    predictorId =    findCorrectPredictor<uint32_t>(data4, totalpredictors[i],
                                                                  (Predictor<uint32_t> **) predictorsListofLists[i]);

                    //write in stream 1
                    //stream1<<predictorId;
                    streams[2*i]<<predictorId;


                    if(predictorId == totalpredictors[i]){
                        //write in stream2
                        tmp = convertToChar<uint32_t>(data4,size);
                        cout<<"4: not able to predict: "<<tmp<<endl;
                        //stream2<<tmp;
                        streams[2*i+1]<<tmp;
                    }
                    else{
                        cout<<"4: correct prediction done by predictor id: "<<predictorId<<endl;
                    }

                    break;
                case 8:
                    data8 = readBytes<uint64>(fstr,size);
                    if(data8 == NULL)
                        break;
                     predictorId = findCorrectPredictor<uint64>(data8, totalpredictors[i],
                                                                (Predictor<uint64> **) predictorsListofLists[i]);
                    //write in stream 3
                    //stream3<<predictorId;
                    streams[2*i]<<predictorId;
                    if(predictorId == totalpredictors[i]){
                        //write in stream4
                        tmp = convertToChar<uint64>(data8,size);
                        cout<<"8: not able to predict: "<<tmp<<endl;
                        //stream4<<tmp;
                        streams[2*i+1]<<tmp;
                    }
                    else{
                        cout<<"8: correct prediction done by predictor id: "<<predictorId<<endl;
                    }
                    break;
                default:
                    cout << "invalid size"<<endl;
            }

        }
    }while(data4 && data8);

    cout<<"Result streams: "<<endl;
    for(int i = 0; i< 2*noOfFields; i++) {

        streams[i].close();
    }
//    cout<<"Stream1: "<<stream1.str()<<endl;
//    cout<<"Stream2: "<<stream2.str()<<endl;
//    cout<<"Stream3: "<<stream3.str()<<endl;
//    cout<<"Stream4: "<<stream4.str()<<endl;



}

template<typename T>
int VPC3::findCorrectPredictor(const T value,unsigned int noOfPredictors,Predictor<T>* predictors[]){
    //cout<<"inside findCorrectPredictor"<<noOfPredictors<<predictors<<"\n\n\n";
    int predictorId = noOfPredictors;
    int usageCount = -1;
    for( int index=0; index < noOfPredictors; index++){
            //cout<<"id: "<<predictors[index]->getPredictorId()<<endl;

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
//        for( int index=0; index < noOfPredictors; index++) {
//            predictors[index]->update(value);
//        }
    }
    return predictorId;
}

template<typename T>
//extra 0 in the end is getting read ## to be fixed
T VPC3::readBytes(ifstream& fstr,int noOfBytes){
    T data =0;
    T tmpdata = 0;
    if(!fstr || fstr.eof()){
        cout<<"file reference not found or file end is encountered "<<endl;
        return NULL;
    }
//    char* buffer;
//    buffer = new char[noOfBytes];
   fstr.read(reinterpret_cast<char*>(&data), noOfBytes);
    std::streamsize bytes = fstr.gcount();
    if(bytes==0){
        return NULL;
    }
   // fstr.read(buffer,noOfBytes);
//    cout<<"loop: "<< buffer<<endl;
    //data=(T)*buffer;

//    fstr.read(reinterpret_cast<char*>(&data),noOfBytes);
//   cout<<"loop: "<< &data<<endl;
//    for(int i =noOfBytes-1; i>=0; i-- ){
//        data = (data << 8)+buffer[i];
//    }
//    Int32 = (Int32 << 8) + MyBytes[3];
//    Int32 = (Int32 << 8) + MyBytes[2];
//    Int32 = (Int32 << 8) + MyBytes[1];
//    Int32 = (Int32 << 8) + MyBytes[0];
//   char* tmp = new char[noOfBytes];
//    tmpdata = data;
//    for(int i =0; i<noOfBytes; i++ ){
//        tmp[i] = (tmpdata)&0xFF;
//        tmpdata = tmpdata >>8;
//   }
    //char* tmp;
    //tmp = convertToChar<T>(data,noOfBytes);
    //cout<<"tmp:data "<<tmp<<" : "<<data<<endl;
   return data;

}

template<typename T>
char * VPC3::convertToChar(T value,int noOfBytes){
    char* stream = new char[noOfBytes];
    for(int i =0; i<noOfBytes; i++ ){
        stream[i] = (value)&0xFF;
        value = value >>8;
    }
    return stream;

}


template<typename T>
T VPC3::convertToT(const char* buffer, int noOfBytes){
    T data =0;
    for(int i =noOfBytes-1; i>=0; i-- ){
        data = (data << 8)+buffer[i];
    }
    //cout<<"original : "<< buffer <<" convert :"<< data <<"original : "<<convertToChar(data,noOfBytes)<<endl;
    return data;
}



