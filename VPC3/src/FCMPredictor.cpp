//
// Created by Shweta Sahu on 10/3/17.
//

#include "FCMPredictor.h"

using namespace std;


template<class T>
int FCMPredictor<T>::updateCount=0;


template<class T>
unsigned long FCMPredictor<T>::getHashValue(T value){
    unsigned long hashValue = 0;
    while (value > 0) {
        hashValue ^= value;
        value = (unsigned long)value >> bits;
    }
    return (hashValue & ((1<<bits)-1)) ;

}




template<class T>
void FCMPredictor<T>::initialise(unsigned long* firstLevel, T** secondLevel,unsigned int p_hashTableSize,unsigned int p_maxOrder,
                              unsigned int p_order,int p_recent,int p_id, int p_maxRef){
    //firsttable, secondtable,hashtablesize,maxorder,order,recent,id,
	//cout<<"\n initialise: "<<p_hashTableSize<<"\t"<<p_maxOrder<<"\t"<<p_order<<"\t"<<p_recent<<"\t"<<p_id;

    firstLevelTable= firstLevel;
    secondLevelTable  = secondLevel;
    order = p_order;
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
    maxRef = p_maxRef;
    //bits = log2(hashTableSize);

};

template<class T>
T FCMPredictor<T>::getPrediction(){
    unsigned long index = firstLevelTable[order];
    T tmp = secondLevelTable[index][recent];
    return tmp;
}

template<class T>
void FCMPredictor<T>::update(const T newValue)
{
    updateCount++;
    if(recent == 0) {
        //for (int i = 0; i < maxOrder; i += 2) {//need to change to maxorder
            unsigned long index = firstLevelTable[order];
            if (secondLevelTable[index][0] != newValue) {
                secondLevelTable[index][1] = secondLevelTable[index][0];
                secondLevelTable[index][0] = newValue;
            }
        //}
    }

    if(updateCount == maxRef){
        unsigned long  hashValue = getHashValue(newValue);
        for (int i = 2; i > 0; i--) {
            firstLevelTable[i] = (firstLevelTable[i - 1]<< 1)^hashValue;
        }
        firstLevelTable[0] = hashValue;
        updateCount = 0;
    }
}

template<class T>
int FCMPredictor<T>::getUsageCount(){
    return usageCount;
};

template<class T>
void FCMPredictor<T>::incrementUsageCount(){
    usageCount++;
}

template<class T>
int FCMPredictor<T>::getPredictorId(){
    return id;
}
