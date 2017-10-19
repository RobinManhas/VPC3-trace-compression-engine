//
// Created by Shweta Sahu on 10/3/17.
//

#ifndef NEW_PREDICTOR_H
#define NEW_PREDICTOR_H


template<class T>
    class Predictor {
    public:
        virtual inline T getPrediction()=0;
        virtual inline int getPredictorId()=0;
        virtual inline int getUsageCount()=0;
        virtual inline void incrementUsageCount()=0;
        virtual inline void update(const T newValue)=0;


    };




#endif //NEW_PREDICTOR_H
