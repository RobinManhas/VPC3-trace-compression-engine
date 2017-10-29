cmake_minimum_required(VERSION 3.8)
project(VPC3)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/Configuration.cpp
        src/Configuration.h
        src/DFCMPredictor.cpp
        src/DFCMPredictor.h
        src/FCMPredictor.cpp
        src/FCMPredictor.h
        src/InputConfigReader.cpp
        src/InputConfigReader.h
        src/LVPredictor.cpp
        src/LVPredictor.h
        src/Predictor.h
        src/testconfig.txt
        src/VPC3.cpp
        src/VPC3.h
        main.cpp)

add_executable(VPC3 ${SOURCE_FILES})