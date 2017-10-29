//
// Created by Shweta Sahu on 10/3/17.
//

#ifndef NEW_FCMPREDICTOR_H
#define NEW_FCMPREDICTOR_H

#include "VPC3.h"
#include "Predictor.h"

using namespace std;
template<class T>
class FCMPredictor: public Predictor<T>{
private:
    unsigned int usageCount=0;
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



#endif //NEW_FCMPREDICTOR_H
