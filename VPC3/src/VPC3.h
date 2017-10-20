//
// Created by Shweta Sahu on 10/3/17.
//

#ifndef NEW_VPC3_H
#define NEW_VPC3_H

#include "Predictor.h"
#include "Configuration.h"
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string.h>


using namespace std;
class VPC3 {

private:
    void ***predictorsListofLists;    // to be


    void preparePredictors(TraceConfig* cfg);

    template<typename T>
    int findCorrectPredictor(const T value,unsigned int noOfPredictors,Predictor<T>* predictors[]);

    template<typename T>
    T readBytes(ifstream& fstr,int noOfBytes);

    template<typename T>
    char * convertToChar(T value,int noOfBytes);

    template<typename T>
    T convertToT(const char* buffer, int noOfBytes);

    template<typename T>
    void updatePredictors(Predictor<T>* predictors[],T value,int noOfPredictors);

    template<typename T>
    T getValueFromPredictor(Predictor<T>* predictors[], int predictorId);


    template <typename T>
    int getLVPredictors(int countOfpredictor, Predictor<T> **predictors,int id);

    template <typename T>
    int getFCMPredictors(Predictor<T> **predictors,int id, int const hashTableSize,map<string, int> fcmOrderMap);

    template <typename T>
    int getDFCMPredictors(Predictor<T> *predictors[],int id, int const hashTableSize,map<string, int> dfcmOrderMap);

        template <typename T>
    Predictor<T> ** assignPredictors(TraceConfig* cfg,int i);

    template <typename T>
    T encodePredictions(ifstream& fstr,TraceConfig* cfg, int i,int size,ofstream streams[]);

    template<typename T>
    T decodePredictions(ifstream streams[],TraceConfig* cfg, int i, int size,ofstream& file);

public:
    void encode(ifstream& fstr,TraceConfig* cfg);
    void decode(ifstream streams[],TraceConfig* cfg);



};


#endif //NEW_VPC3_H
