//
// Created by Shweta Sahu on 10/19/17.
//

#ifndef VPC3_DFCMPREDICTOR_H
#define VPC3_DFCMPREDICTOR_H

#include <stdio.h>
#include "Predictor.h"

using namespace std;



template<class T>
class DFCMPredictor: public Predictor<T>{
private:
    unsigned int usageCount;
    unsigned int order;
    unsigned int hashTableSize;
    int id;
    int recent;
    int maxOrder;
    int bits=-1;
    unsigned long* firstLevelTable;
    T** secondLevelTable;//contains most recent value that followed the input , second recent
    unsigned long getHashValue(T value);
    static int updateCount;
    int maxRef;

public:

    void initialise(unsigned long* firstLevel, T** secondLevel,unsigned int hashTableSize,unsigned int maxOrder,
                    unsigned int order,int recent,int id, int p_maxRef);
    T getPrediction();
    void update(const T newValue);
    int getUsageCount();
    void incrementUsageCount();
    int getPredictorId();
};


#endif //VPC3_DFCMPREDICTOR_H
