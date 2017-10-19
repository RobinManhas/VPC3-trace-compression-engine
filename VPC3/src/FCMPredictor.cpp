//
// Created by Shweta Sahu on 10/3/17.
//
#include <iostream>
#include "FCMPredictor.h"

using namespace std;


template<class T>
int FCMPredictor<T>::updateFlag=0;

template<class T>
void FCMPredictor<T>::setUpdateFlag(){
    updateFlag = 1;
}

template<class T>
unsigned int FCMPredictor<T>::getHashValue(unsigned int value){
    unsigned int hashValue = 0;
    while (value > 0) {
        hashValue = (unsigned int)hashValue ^ (unsigned int)value;
        value = value >> sizeof(unsigned int);
    }
    return hashValue%hashTableSize ;
}




template<class T>
void FCMPredictor<T>::initialise(int* firstLevel, T** secondLevel,unsigned int p_hashTableSize,unsigned int p_maxOrder,
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
T FCMPredictor<T>::getPrediction(){
    unsigned int index = firstLevelTable[order-1];
    T value = -1;
    switch(recent){//'a' specifies most recent ang 'b': second most recent
        case 'a' : value = secondLevelTable[index][0];break;
        case 'b' : value =  secondLevelTable[index][1];break;
    }
    return value;

}

template<class T>
void FCMPredictor<T>::update(const T newValue)
{
    unsigned int  hashValue = getHashValue(newValue);

    //if(firstLevelTable[0] != hashValue){//removed
    if(updateFlag == 1){
        //cout << "inside update for FCM :"<<id<<" : "<< updateFlag<<endl;
        for(int i=0; i<3; i++) {//need to change to maxorder
            unsigned int index = firstLevelTable[i];
            if (secondLevelTable[index][0] != newValue) {
                secondLevelTable[index][1] = secondLevelTable[index][0];
                secondLevelTable[index][0] = newValue;
            }
        }

        for (int i = 2; i > 0; i--) {
            firstLevelTable[i] = firstLevelTable[i - 1]^hashValue;
        }
        firstLevelTable[0] = hashValue;
        updateFlag = 0;
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
