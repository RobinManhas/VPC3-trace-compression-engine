//
// Created by Shweta Sahu on 10/3/17.
//
#include <iostream>
#include "DFCMPredictor.h"

using namespace std;


template<class T>
int DFCMPredictor<T>::updateFlag=0;

template<class T>
void DFCMPredictor<T>::setUpdateFlag(){
    updateFlag = 1;
}

template<class T>
unsigned int DFCMPredictor<T>::getHashValue(unsigned int value){
    unsigned int hashValue = 0;
    while (value > 0) {
        hashValue = (unsigned int)hashValue ^ (unsigned int)value;
        value = value >> sizeof(unsigned int);
    }
    return hashValue%hashTableSize ;
}




template<class T>
void DFCMPredictor<T>::initialise(int* firstLevel, T** secondLevel,unsigned int p_hashTableSize,unsigned int p_maxOrder,
                                 unsigned int p_order,char p_recent,int p_id){
    //firsttable, secondtable,hashtablesize,maxorder,order,recent,id,
    //cout<<"\n initialise: "<<p_hashTableSize<<"\t"<<p_maxOrder<<"\t"<<p_order<<"\t"<<p_recent<<"\t"<<p_id;
    firstLevelTable= firstLevel;
    secondLevelTable  = secondLevel;
    order = p_order;
    hashTableSize = p_hashTableSize;
    id= p_id;
    recent= p_recent;
    maxOrder = p_maxOrder;

};

template<class T>
T DFCMPredictor<T>::getPrediction(){
    unsigned int index = firstLevelTable[order];
    T value = -1;
    switch(recent){//'a' specifies most recent ang 'b': second most recent
        case 'a' : value = secondLevelTable[index][0]+firstLevelTable[0];break;  // as strides were saved, so now adding the last value
        case 'b' : value =  secondLevelTable[index][1]+firstLevelTable[0];break;
    }
    return value;

}

template<class T>
void DFCMPredictor<T>::update(const T newValue)
{
    unsigned int  hashValue = getHashValue(newValue);

    //if(firstLevelTable[0] != hashValue){//removed
    if(updateFlag == 1){
        //cout << "inside update for FCM :"<<id<<" : "<< updateFlag<<endl;
        for(int i=1; i <= maxOrder; i++) {

            unsigned int index = firstLevelTable[i];
            if (secondLevelTable[index][0] != newValue) {
                secondLevelTable[index][1] = secondLevelTable[index][0];
                secondLevelTable[index][0] = newValue;
            }
        }

        for (int i = maxOrder; i > 1; i--) {
            firstLevelTable[i] = ((firstLevelTable[i - 1]+firstLevelTable[0])^hashValue)-newValue;
        }
        firstLevelTable[1] = hashValue - newValue;
        firstLevelTable[0] = newValue;
        updateFlag = 0;
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
