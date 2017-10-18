//
// Created by Shweta Sahu on 10/3/17.
//

#ifndef NEW_LVPREDICTOR_H
#define NEW_LVPREDICTOR_H

#include "Predictor.h"

template<class T>
class LVPredictor: public Predictor<T>{
private:
    int size;
    T* table;
    unsigned int usageCount;
    int id;

public:
    void initialise(T* table,int size,int id);
    T getPrediction();
    void update(const T newValue);
    int getUsageCount();
    void incrementUsageCount();
    int getPredictorId();
};




#endif //NEW_LVPREDICTOR_H
