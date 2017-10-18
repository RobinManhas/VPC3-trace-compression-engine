//
// Created by Shweta Sahu on 10/3/17.
//

#ifndef NEW_VPC3_H
#define NEW_VPC3_H



#include "Predictor.h"
#include "Configuration.h"
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string.h>

using namespace std;
class VPC3 {

private:
//	unsigned static int const noOfFields = 2;//input
	void **predictorsListofLists[2];    // 2 sets of predictors one for PC and one for ED
//	int sizes[noOfFields] = {4, 8}; // from input
	unsigned short totalpredictors[2]={4,6};  // 4 predictors for PC and 6 for ED
    int test();

    void prepare();

    template<typename T>
    int findCorrectPredictor(const T value,unsigned int noOfPredictors,Predictor<T>* predictors[]);

    template<typename T>
    T readBytes(ifstream& fstr,int noOfBytes);

    template<typename T>
    char * convertToChar(T value,int noOfBytes);

    template<typename T>
    T convertToT(const char* buffer, int noOfBytes);



    void decodeStreams(ifstream streams[],TraceConfig* cfg);


    template<typename T>
    void updatePredictors(Predictor<T>* predictors[],T value,int noOfPredictors);

    template<typename T>
    T getValueFromPredictor(Predictor<T>* predictors[], int predictorId);


public:
    void encode(ifstream& fstr,TraceConfig* cfg);
    void decode(ifstream streams[],TraceConfig* cfg);



};


#endif //NEW_VPC3_H
