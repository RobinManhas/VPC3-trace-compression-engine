//
// Created by Shweta Sahu on 10/3/17.
//

#ifndef NEW_VPC3_H
#define NEW_VPC3_H



#include "Predictor.h"
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string.h>

using namespace std;
class VPC3 {

private:
	unsigned static int const noOfFields = 2;//input
	void **predictorsListofLists[noOfFields];    // 2 sets of predictors one for PC and one for ED
	int sizes[noOfFields] = {4, 8}; // from input
	unsigned short totalpredictors[noOfFields]={4,6};  // 4 predictors for PC and 6 for ED


//	ostringstream stream1;//for predictor code for 4byte data
//	ostringstream stream2;//for unpredicted 4 byte data
//	ostringstream stream3;//for predictor code for 8byte data
//	ostringstream stream4;//for unpredicted 8 byte data

    //char noOfBytes;
//returns the predictor id

// sizeInBits : 32 or 64
/*   void tmp(short sizeInBits){
//    	noOfBytes = sizeInBits/4;
//    		switch(sizeInBits){
//    		case 32:
//    			typedef uint32_t T;
//    			unsigned char totalNoOfPredictors = 4;
//    		 	Predictor<T>* predictors[4];
//
//    			break;
//    		case 64: typedef uint64_t T1;
//    				totalNoOfPredictors = 10;
//    				break;
//    		}


 //   }*/

public:
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

	void encode(ifstream& fstr);

	void decodeStreams(ifstream streams[]);

	void decode(ifstream streams[]);
	template<typename T>
	void updatePredictors(Predictor<T>* predictors[],T value,int noOfPredictors);

	template<typename T>
	T getValueFromPredictor(Predictor<T>* predictors[], int predictorId);


};


#endif //NEW_VPC3_H
