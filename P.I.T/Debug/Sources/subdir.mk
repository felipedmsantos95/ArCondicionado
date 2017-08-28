################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
C:/Users/leona/Documents/KL25Z\ Peripherals\ for\ C++/Microcontroller\ Kinetis\ Low\ power\ (MKL)/PIT\ for\ KL25Z/PIT\ Periodic\ Interrupt\ -\ v1.0,\ 2017-2/main.cpp \
C:/Users/leona/Documents/KL25Z\ Peripherals\ for\ C++/Microcontroller\ Kinetis\ Low\ power\ (MKL)/PIT\ for\ KL25Z/PIT\ Periodic\ Interrupt\ -\ v1.0,\ 2017-2/mkl_PIT.cpp \
C:/Users/leona/Documents/KL25Z\ Peripherals\ for\ C++/Microcontroller\ Kinetis\ Low\ power\ (MKL)/PIT\ for\ KL25Z/PIT\ Periodic\ Interrupt\ -\ v1.0,\ 2017-2/mkl_PITPeriodicInterrupt.cpp 

OBJS += \
./Sources/main.o \
./Sources/mkl_PIT.o \
./Sources/mkl_PITPeriodicInterrupt.o 

CPP_DEPS += \
./Sources/main.d \
./Sources/mkl_PIT.d \
./Sources/mkl_PITPeriodicInterrupt.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/main.o: C:/Users/leona/Documents/KL25Z\ Peripherals\ for\ C++/Microcontroller\ Kinetis\ Low\ power\ (MKL)/PIT\ for\ KL25Z/PIT\ Periodic\ Interrupt\ -\ v1.0,\ 2017-2/main.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"../Sources" -I"../Includes" -std=gnu++11 -fabi-version=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/mkl_PIT.o: C:/Users/leona/Documents/KL25Z\ Peripherals\ for\ C++/Microcontroller\ Kinetis\ Low\ power\ (MKL)/PIT\ for\ KL25Z/PIT\ Periodic\ Interrupt\ -\ v1.0,\ 2017-2/mkl_PIT.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"../Sources" -I"../Includes" -std=gnu++11 -fabi-version=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sources/mkl_PITPeriodicInterrupt.o: C:/Users/leona/Documents/KL25Z\ Peripherals\ for\ C++/Microcontroller\ Kinetis\ Low\ power\ (MKL)/PIT\ for\ KL25Z/PIT\ Periodic\ Interrupt\ -\ v1.0,\ 2017-2/mkl_PITPeriodicInterrupt.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"../Sources" -I"../Includes" -std=gnu++11 -fabi-version=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


