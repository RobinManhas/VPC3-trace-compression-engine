//
// Created by Shweta Sahu on 10/19/17.
//

#ifndef VPC3_DFCMPREDICTOR_H
#define VPC3_DFCMPREDICTOR_H

#include <stdio.h>
#include "main.h"
#include "Predictor.h"

using namespace std;
template<class T>
class DFCMPredictor: public Predictor<T>{
private:
    unsigned int usageCount;
    unsigned int order;
    unsigned int hashTableSize;
    int id;
    char recent;
    int maxOrder;
    int* firstLevelTable;
    T** secondLevelTable;//contains most recent value that followed the input , second recent
    unsigned int getHashValue(unsigned int value);
    static int updateFlag;

public:

    void initialise(int* firstLevel, T** secondLevel,unsigned int hashTableSize,unsigned int maxOrder,
                    unsigned int order,char recent,int id);
    T getPrediction();
    void update(const T newValue);
    int getUsageCount();
    void incrementUsageCount();
    int getPredictorId();
    static void setUpdateFlag();
};


#endif //VPC3_DFCMPREDICTOR_H