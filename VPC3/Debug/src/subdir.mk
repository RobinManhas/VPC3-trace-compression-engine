################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/FCMPredictor.cpp \
../src/LVPredictor.cpp \
../src/Predictor.cpp \
../src/VPC3.cpp \
../src/main.cpp 

OBJS += \
./src/FCMPredictor.o \
./src/LVPredictor.o \
./src/Predictor.o \
./src/VPC3.o \
./src/main.o 

CPP_DEPS += \
./src/FCMPredictor.d \
./src/LVPredictor.d \
./src/Predictor.d \
./src/VPC3.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


