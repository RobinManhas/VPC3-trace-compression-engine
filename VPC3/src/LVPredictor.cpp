//
// Created by Shweta Sahu on 10/3/17.
//

#include "LVPredictor.h"

template<class T>
void LVPredictor<T>::initialise(T* p_table,int p_size,int p_id){
    table = p_table;
    size = p_size;
    id = p_id;
    usageCount = 0;

}

template<class T>
T LVPredictor<T>::getPrediction(){
    return table[id];
}

template<class T>
void LVPredictor<T>::update(const T newValue){
    if (newValue != table[0]){
        int i = size-1;
        for( ; i > 0; i-- ){
            table[i] = table[i-1];
        }
        table[i] = newValue;
    }
}

template<class T>
int LVPredictor<T>::getUsageCount(){
    return usageCount;
};

template<class T>
void LVPredictor<T>::incrementUsageCount(){
    usageCount++;
}

template<class T> int LVPredictor<T>::getPredictorId(){
    return id;
}
