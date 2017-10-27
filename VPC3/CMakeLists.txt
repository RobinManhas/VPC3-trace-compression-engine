cmake_minimum_required(VERSION 3.8)
project(VPC3)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/FCMPredictor.cpp
        src/FCMPredictor.h
        src/LVPredictor.cpp
        src/LVPredictor.h
        src/Predictor.h
        src/VPC3.cpp
        src/VPC3.h
        src/Configuration.cpp
        src/Configuration.h
        src/InputConfigReader.cpp
        src/InputConfigReader.h
        src/DFCMPredictor.cpp
        src/DFCMPredictor.h)


add_executable(VPC3 ${SOURCE_FILES})