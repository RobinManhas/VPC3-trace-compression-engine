//
// Created by Shweta Sahu on 10/3/17.
//
#include <iostream>
#include "DFCMPredictor.h"

using namespace std;

template<class T>
int DFCMPredictor<T>::updateCount=0;


template<class T>
unsigned long DFCMPredictor<T>::getHashValue(T value){
    unsigned long hashValue = 0;
    while (value > 0) {
        hashValue ^= value;
        value = (unsigned long)value >> bits;
    }
    return (hashValue & ((1<<bits)-1)) ;
}




template<class T>
void DFCMPredictor<T>::initialise(unsigned long* firstLevel, T** secondLevel,unsigned int p_hashTableSize,unsigned int p_maxOrder,
                                 unsigned int p_order,int p_recent,int p_id, int p_maxRef){
    //firsttable, secondtable,hashtablesize,maxorder,order,recent,id,
    //cout<<"\n initialise: "<<p_hashTableSize<<"\t"<<p_maxOrder<<"\t"<<p_order<<"\t"<<p_recent<<"\t"<<p_id;
    firstLevelTable= firstLevel;
    secondLevelTable  = secondLevel;
    order = p_order+1;
    hashTableSize = p_hashTableSize;
    id= p_id;
    recent= p_recent;
    maxOrder = p_maxOrder;
    unsigned int n = hashTableSize;
    if(n&(n-1)){
        bits=0;
    }
    while(n!=0){
        bits++;
        n=n>>1;
    }
    usageCount = 0;
    maxRef = p_maxRef;
};

template<class T>
T DFCMPredictor<T>::getPrediction(){
    unsigned long index = firstLevelTable[order];
    return (secondLevelTable[index][recent]+firstLevelTable[0]);
}

template<class T>
void DFCMPredictor<T>::update(const T newValue)
{

    updateCount++;
    if(recent == 0) {
        unsigned int index = firstLevelTable[order];
        if (secondLevelTable[index][0] != (newValue - firstLevelTable[0])) {
            secondLevelTable[index][1] = secondLevelTable[index][0];
            secondLevelTable[index][0] = (newValue -firstLevelTable[0]);
        }

    }

    if(updateCount == maxRef){
        unsigned long  hashValue = getHashValue(newValue - firstLevelTable[0]);
        for (int i = maxOrder; i > 1; i--) {
            firstLevelTable[i] = (firstLevelTable[i - 1]<< 1)^hashValue;
        }
        firstLevelTable[1] = hashValue;
        firstLevelTable[0] = newValue;
        updateCount = 0;
    }


}

template<class T>
int DFCMPredictor<T>::getUsageCount(){
    return usageCount;
};

template<class T>
void DFCMPredictor<T>::incrementUsageCount(){
    usageCount++;
}

template<class T>
int DFCMPredictor<T>::getPredictorId(){
    return id;
}
