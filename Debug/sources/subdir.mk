################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../sources/AsioUdpClient.cpp \
../sources/Client1.cpp 

OBJS += \
./sources/AsioUdpClient.o \
./sources/Client1.o 

CPP_DEPS += \
./sources/AsioUdpClient.d \
./sources/Client1.d 


# Each subdirectory must supply rules for building sources it contributes
sources/%.o: ../sources/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I.././ -I/usr/include -I/usr/include/c++/4.9/ -I/usr/local/boost_1_59_0 -O0 -g3 -Wall -c -fmessage-length=0 -Xlinker --verbose -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


