//
// Created by Shweta Sahu on 10/3/17.
//

#ifndef NEW_FCMPREDICTOR_H
#define NEW_FCMPREDICTOR_H

#include <vector>
#include "main.h"
#include "Predictor.h"

using namespace std;
template<class T>
class FCMPredictor: public Predictor<T>{
private:
    unsigned int usageCount;
    unsigned int order;
    unsigned int hashTableSize;
    int id;
    char recent;
    uint32_t maxOrder;
    uint32_t* firstLevelTable;
    T** secondLevelTable;//contains most recent value that followed the input , second recent
    unsigned int getHashValue(unsigned int value);
    static int updateFlag;

public:

    void initialise(uint32_t* firstLevel, T** secondLevel,unsigned int hashTableSize,unsigned int maxOrder,
                    unsigned int order,char recent,int id);
    T getPrediction();
    void update(const T newValue);
    int getUsageCount();
    void incrementUsageCount();
    int getPredictorId();
    static void setUpdateFlag();
};

//#include "FCMPredictor.cpp"


#endif //NEW_FCMPREDICTOR_H
