#include "VPC3.h"
#include "FCMPredictor.h"
#include "DFCMPredictor.h"
#include "LVPredictor.h"
#include "Predictor.h"
#include "FCMPredictor.cpp"
#include "DFCMPredictor.cpp"
#include "LVPredictor.cpp"
#include "InputConfigReader.h"
using namespace std;


/*
 *  * *********************************   PREDICTORS  START ******************************************************
 */



void VPC3::preparePredictors(TraceConfig* cfg) {
    int noOfFields = cfg->getFields().size();
    sizes = new int[noOfFields];
    predictorsListofLists = new void**[noOfFields];
    totalPredictors = new int[noOfFields];
    for(int i = 0; i<noOfFields; i++) {
        totalPredictors[i] = cfg->getFields()[i]->totalPredictors;
        sizes[i] = cfg->getFields()[i]->iFieldLen;
        switch (sizes[i]) {
            case 4:
                predictorsListofLists[i] = (void **) assignPredictors<uint32_t>(cfg, i);
                break;
            case 8:
                predictorsListofLists[i] = (void **) assignPredictors<uint64>(cfg, i);
                break;
        }
    }
}

template <typename T>
Predictor<T> ** VPC3::assignPredictors(TraceConfig* cfg,int i) {
    int counter = 0;
    int countOfpredictor = 0;
    int id = 0;
    Predictor<T> **predictors= new Predictor<T>*[totalPredictors[i]];
    map<string, int>::iterator it;
    bool isFCMPredictorsPresent = false;
    map<string, int> fcmOrderMap;
    int totalFCMPredictors = 0;

    bool isDFCMPredictorsPresent = false;
    map<string, int> dfcmOrderMap;
    int totalDFCMPredictors = 0;

    for (it = cfg->getFields()[i]->mPredictorMap.begin(), counter = 0;
         it != cfg->getFields()[i]->mPredictorMap.end(); it++, counter++) {

        countOfpredictor = it->second;

        if (it->first == LV) {
            getLVPredictors<T>(countOfpredictor, predictors, id);
            id = id + countOfpredictor;

        } else if (it->first.substr(0,3) == "FCM") {
            isFCMPredictorsPresent = true;
            string order = it->first.substr(3,1);
            fcmOrderMap.insert(make_pair(order, it->second));
            totalFCMPredictors += it->second;

        }else if (it->first.substr(0,4) == "DFCM") {
            isDFCMPredictorsPresent = true;
            string order = it->first.substr(4,1);
            dfcmOrderMap.insert(make_pair(order, it->second));
            totalDFCMPredictors += it->second;
        }

    }
    if(isFCMPredictorsPresent){
        id = getFCMPredictors(predictors,id,cfg->getFields()[i]->iL2,fcmOrderMap);
    }
    if(isDFCMPredictorsPresent){
        id = getDFCMPredictors(predictors,id,cfg->getFields()[i]->iL2,dfcmOrderMap);
    }

    return predictors;
}


template <typename T>
int VPC3::getLVPredictors(int countOfpredictor, Predictor<T> *predictors[],int id){

    T *lvTable= new T[countOfpredictor];
    for (int i = 0; i < countOfpredictor; i++) {
        lvTable[i] = 0;
    }
    for(int z = 0; z< countOfpredictor; z++){
        LVPredictor<T> *l = new LVPredictor<T>();
        l->initialise(lvTable, countOfpredictor, id);
        predictors[id++]=l;

    }
    return id;
}

template <typename T>
int VPC3::getFCMPredictors(Predictor<T> *predictors[],int id, int const hashTableSize,map<string, int> fcmOrderMap){

    int  maxOrder = 3; // to be processed;
    //char recent[] = {'a','b'};

    unsigned long *firstLevelTable = new unsigned long[maxOrder];
    for (int i = 0; i < maxOrder; i++) {
        firstLevelTable[i] = 0;
    }
    map<string, int>::iterator it;
    for (it = fcmOrderMap.begin(); it != fcmOrderMap.end(); it++) {
        int countOfpredictor = it->second;
        int order = stoi(it->first)-1;

        unsigned long long size = hashTableSize * (1<<order);
        T** secondLevelTable = new T*[size];

        for (unsigned long long i = 0; i < size; i++) {
            secondLevelTable[i] = new T[2];
            secondLevelTable[i][0] = 0;
            secondLevelTable[i][1] = 1;

        }
        for (int z = 0; z < countOfpredictor; z++) {
            FCMPredictor<T> *f = new FCMPredictor<T>();
            f->initialise(firstLevelTable,secondLevelTable, hashTableSize, maxOrder, order, z, id,countOfpredictor);
            predictors[id++] = f;
        }
    }
    return id;

}

template <typename T>
int VPC3::getDFCMPredictors(Predictor<T> *predictors[],int id, int const hashTableSize,map<string, int> dfcmOrderMap){

    int  maxOrder = 3;
    T *secondLevelTable[hashTableSize];

    for (T i = 0; i < hashTableSize; i++) {
        secondLevelTable[i] = new T[2];
        for (T j = 0; j < 2; j++) {
            secondLevelTable[i][j] = 0;
        }
    }

    unsigned long *firstLevelTable = new unsigned long[maxOrder+1];
    for (int i = 0; i <=  maxOrder; i++) {
        firstLevelTable[i] = 0;
    }
    map<string, int>::iterator it;
    for (it = dfcmOrderMap.begin(); it != dfcmOrderMap.end(); it++) {
        int countOfpredictor = it->second;
        int order = stoi(it->first)-1;

        unsigned long long size = hashTableSize * (1<<order);
        T** secondLevelTable = new T*[size];

        for (unsigned long long i = 0; i < size; i++) {
            secondLevelTable[i] = new T[2];
            secondLevelTable[i][0] = 0;
            secondLevelTable[i][1] = 1;

        }
        for (int z = 0; z < countOfpredictor; z++) {
            DFCMPredictor<T> *d = new DFCMPredictor<T>();
            d->initialise(firstLevelTable, secondLevelTable, hashTableSize, maxOrder, order, z, id,countOfpredictor);
            predictors[id++] = d;
        }
    }
    return id;

}

template<typename T>
void VPC3::updatePredictors(Predictor<T>* predictors[],T value, int totalPred){
    for( int index=totalPred-1; index >= 0; index--) {
        predictors[index]->update(value);
    }

}

template<typename T>
T VPC3::getValueFromPredictor(Predictor<T>* predictors[], int predictorId){
    return predictors[predictorId]->getPrediction();
}

template<typename T>
int VPC3::findCorrectPredictor(const T value,Predictor<T>* predictors[],int totalPred){
    int predictorId = totalPred;
    int usageCount = -1;
    for( int index=0; index < totalPred; index++){
        if((predictors[index]->getUsageCount() > usageCount) && (predictors[index]->getPrediction() == value)){
            predictorId = predictors[index]->getPredictorId();
            usageCount = predictors[index]->getUsageCount();
        }
    }
    updatePredictors(predictors,value,totalPred);
    if(predictorId!=totalPred)
    predictors[predictorId]->incrementUsageCount();

    return predictorId;
}


/////////////////////////////////////// PREDICTORS END ////////////////////////////////////////////////

/*
 * *********************************   ENCODING  START ******************************************************
 */

int VPC3::compress(FILE *input_file, string cfgFile, string output_path){
    TraceConfig* cfg = parseFile(cfgFile);
    if(cfg == NULL || input_file==NULL || output_path == ""){
        cout<<"Input is null"<<endl;
        return -1;
    }
    //predictors initialisation
    preparePredictors(cfg);

    int noOfFields = cfg->getFields().size();
    FILE** output_streams;
    output_streams = new FILE*[2*noOfFields];

    int tmp_size=0;

    lengths=new int[2*noOfFields];
    stream_buffer = new char*[2*noOfFields];

    for(int i = 0; i< 2*noOfFields; i++) {
        string suffix = cfg->getCompressor();
        suffix.append("> ");
        suffix.append(output_path);
        suffix.append(PathSeparator);
        suffix.append("stream");
        suffix.append(1,i+'0');

        output_streams[i]=popen(suffix.c_str(),"w");
        if(i%2==0){
            stream_buffer[i]=new char[BUFFERSIZE];
        }else{
            tmp_size=sizes[i/2];
            buffer_size+=tmp_size;
            stream_buffer[i]=new char[BUFFERSIZE*tmp_size];
        }
        lengths[i]=0;

    }
    buffer_size*=BUFFERSIZE;
    buffer = new char[buffer_size];


    cout<<"Compression started.."<<endl;
    int readCOunt =0;
    readCOunt = fread(buffer,1,buffer_size,input_file);
    do {

        for (int i = 0; i < noOfFields; i++) {
            switch (sizes[i]) {
                case 4:
                    encodePredictions<uint32_t>(i, sizes[i],output_streams);
                    break;
                case 8:
                    encodePredictions<uint64>(i, sizes[i],output_streams);
                    break;
                default:
                    cout << "invalid size"<<endl;
            }
            len +=sizes[i];
            if(len>=readCOunt){
                readCOunt = fread(buffer,1,buffer_size,input_file);
                len=0;
            }

        }
    }while(len<readCOunt);

    for(int i = 0; i< 2*noOfFields; i++) {
        if(lengths[i]>0){
            fwrite(stream_buffer[i], 1, lengths[i], output_streams[i]);
        }
        pclose(output_streams[i]);
    }
    cout<<"Compression done successfully"<<endl;
    return 0;

}

template <typename T>
T VPC3::encodePredictions(int i,int size,FILE** output_streams){

    int j =size-1;
    T data = buffer[len+j];
    j--;
    while(j>=0){
        data = (data<<8)|buffer[len+j];
        j--;
    }

    int predictorId =    findCorrectPredictor<T>(data,(Predictor<T> **) predictorsListofLists[i],totalPredictors[i]);
    //write in first stream
    char encodedId = predictorId;
    int s = 2*i;
    stream_buffer[s][lengths[s]++]=encodedId;
    if(lengths[s]==BUFFERSIZE){
        fwrite(stream_buffer[s], sizeof(char), lengths[s], output_streams[s]);
        lengths[s]=0;
    }



    if(predictorId == totalPredictors[i]){
        //write in second stream
        s+=1;
       // char * tmp = convertToChar<T>(data,size);
        for(j=0;j<size;j++){
            stream_buffer[s][lengths[s]++]=data;
            data=data>>8;
        }
        if(lengths[s]==BUFFERSIZE*size){
            fwrite(stream_buffer[s], sizeof(char), lengths[s], output_streams[s]);
            lengths[s]=0;
        }

    }

    return data;
}

//////////////////////////////////////// ENCODING END ///////////////////////////////////////////////////

/*
 *  * *********************************   DECODING  START ******************************************************
 */

int VPC3::decompress(string input, string cfgFile, char *output_file){
    TraceConfig* cfg = parseFile(cfgFile);
    if(cfg == NULL || input == "" || output_file == NULL){
        cout<<"Input is null"<<endl;
        return -1;
    }
    preparePredictors(cfg);
    FILE* output = fopen(output_file,"w");
    int noOfFields = cfg->getFields().size();
    lengths=new int[2*noOfFields];
    FILE** input_streams;
    input_streams = new FILE*[2*noOfFields];
    stream_buffer = new char*[2*noOfFields];
    int tmp_size=0;

    for(int i = 0; i< 2*noOfFields; i++) {
        string suffix = cfg->getDecompressor();
        suffix.append("< ");
        suffix.append(input);
        suffix.append(PathSeparator);
        suffix.append("stream");
        suffix.append(1,i+'0');

        input_streams[i]=popen(suffix.c_str(),"r");
        if(i%2==0){
            stream_buffer[i]=new char[BUFFERSIZE];
            len=fread(stream_buffer[i],1,BUFFERSIZE,input_streams[i]);
            if(i==0){
                reads=len;
            }
        }else{
            tmp_size=sizes[i/2];
            buffer_size+=tmp_size;
            stream_buffer[i]=new char[BUFFERSIZE*tmp_size];
            fread(stream_buffer[i],1,BUFFERSIZE*tmp_size,input_streams[i]);
        }
        lengths[i]=0;

    }
    len=0;
    buffer = new char[BUFFERSIZE*buffer_size];
    cout<<"Decompression started.."<<endl;
    do {
        for (int i = 0; i < noOfFields; i++) {
            int size = sizes[i];
            switch (size) {
                case 4:
                    decodePredictions<uint32_t>(i,size,input_streams);

                    break;
                case 8:
                    decodePredictions<uint64>(i,size,input_streams);
                    break;
                default:
                    cout << "invalid size"<<endl;
            }
            if(len==BUFFERSIZE*buffer_size){
                fwrite(buffer,1,len,output);
                len=0;
            }
        }
    }while(lengths[0]<reads);
    for(int i = 0; i< 2*noOfFields; i++) {
        pclose(input_streams[i]);
    }
    if(len>0){
        fwrite(buffer,1,len,output);
    }
    pclose(output);

    cout<<"Decompression done successfully"<<endl;
    return 0;

}

template <typename T>
T VPC3::decodePredictions(int i,int size,FILE **input_streams) {
    T data;
    int i_tmp=2*i;
    int j=0;
    int predictorId = stream_buffer[i_tmp][lengths[i_tmp]++];
    if(lengths[i_tmp]==BUFFERSIZE){
        j=fread(stream_buffer[i_tmp],1,BUFFERSIZE,input_streams[i_tmp]);
        if(i_tmp==0){
            reads=j;
        }
        lengths[i_tmp]=0;
    }
    i_tmp+=1;
    if(predictorId>=0){
        if(predictorId == totalPredictors[i]){
            data = readBytes<T>(stream_buffer[i_tmp],lengths[i_tmp],size);
            lengths[i_tmp]+=size;
            if(lengths[i_tmp]==BUFFERSIZE*size){
                fread(stream_buffer[i_tmp],1,lengths[i_tmp],input_streams[i_tmp]);
                lengths[i_tmp]=0;
            }

        }
        else{
            //take value from predictor
            data = getValueFromPredictor<T>((Predictor<T> **)predictorsListofLists[i],predictorId);


        }
        updatePredictors((Predictor<T> **) predictorsListofLists[i],data,totalPredictors[i]);
        while(size>0){
            buffer[len++]=data&0xFF;
            data=data>>8;
            size--;
        }
//        file<<convertToChar<T>(data,size);
    }
    return data;
}

//////////////////////////////////////// ENCODING END ///////////////////////////////////////////////////


/*
 *  * *********************************   UTILS  START ******************************************************
 */

template<typename T>
char * VPC3::convertToChar(T value,int noOfBytes){
    char* stream = new char[noOfBytes+1];
    for(int i =0; i<noOfBytes; i++ ){
        stream[i] = (value)&0xFF;
        value = value >>8;
    }
    stream[noOfBytes]='\0';
    return stream;

}
template<typename T>
T VPC3::readBytes(char* stream_buffer,int lenght,int noOfBytes){
    T data =0;
    for(int i =lenght+noOfBytes-1; i>=lenght; i-- ){
        data = (data << 8)|stream_buffer[i];
    }
    return data;

}
template<typename T>
T VPC3::convertToT(const char* buffer, int noOfBytes){
    T data =0;
    for(int i =noOfBytes-1; i>=0; i-- ){
        data = (data << 8)+buffer[i];
    }
    return data;
}

void VPCCommit(const void * const buf, const int elemsize, const int count, FILE * const f, const char * const str)
{
    if (fwrite(buf, elemsize, count, f) != count) {
        cout<<"error writing to "<<str<<endl;
        exit(EXIT_FAILURE);
    }
}



