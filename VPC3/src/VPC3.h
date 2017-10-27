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


#ifdef _WIN32
#define PathSeparator "\\"
#else
#define PathSeparator "/"
#endif
#define BUFFERSIZE 8192
static const string LV = "LV";
static const string FCM="FCM";
static const string DFCM="DFCM";
static const string FCM1="FCM1";
static const string DFCM1="DFCM1";
static const string FCM3="FCM3";
static const string DFCM3="DFCM3";
typedef unsigned long uint64 ;
using namespace std;
class VPC3 {
private:

    char * buffer;
    int len=0;
    int reads=0;
    int *lengths;
    int* sizes;
    int buffer_size=0;
    int* totalPredictors;
    char** stream_buffer;

    void ***predictorsListofLists;    // to be


    void preparePredictors(TraceConfig* cfg);

    template<typename T>
    int findCorrectPredictor(const T value,Predictor<T>* predictors[], int totalPred);

    template<typename T>
    T readBytes(FILE* fstr,int noOfBytes);

    template<typename T>
    T readBytes(char* stream_buffer,int lenght,int noOfBytes);

    template<typename T>
    char * convertToChar(T value,int noOfBytes);

    template<typename T>
    T convertToT(const char* buffer, int noOfBytes);

    template<typename T>
    void updatePredictors(Predictor<T>* predictors[],T value, int totalPred);

    template<typename T>
    T getValueFromPredictor(Predictor<T>* predictors[], int predictorId);


    template <typename T>
    int getLVPredictors(int countOfpredictor, Predictor<T> **predictors,int id);

    template <typename T>
    int getFCMPredictors(Predictor<T> **predictors,int id, int const hashTableSize,map<string, int> fcmOrderMap);

    template <typename T>
    int getDFCMPredictors(Predictor<T> *predictors[],int id, int const hashTableSize,map<string, int> dfcmOrderMap);

    template <typename T>
    Predictor<T> ** assignPredictors(TraceConfig* cfg,int i);

    template <typename T>
    T encodePredictions(int i,int size,FILE* streams[]);

    template<typename T>
    T decodePredictions(int i, int size,FILE** input_streams);

public:
    /// Function to encode and compress a file
    /// \param input_file  FILE to encode and compress
    /// \param cfgFile   configutaion file, where the type of input and predictors used etc. is defined
    /// \param output_path location where compressed streams(stream0, stream1.... ) to be saved
    int compress(FILE *input_file, string cfgFile, string output_path);

    /// Function to decompress and decode the compressed streams (compression must be done using VPC3 itself)
    /// \param path location where streams are stored, which has to be decompressed(where stream0, stream1.... are saved)
    /// \param cfgFile configutaion file, where the type of input and predictors used etc. is defined
    /// \param output_file the full filename where the output will be saved
    int decompress(string path, string cfgFile, char *output_file);



};
#endif //NEW_VPC3_H
