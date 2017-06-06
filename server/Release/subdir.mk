################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/dushn/diplomski\ /making_chat_great_again/shot/server2.c 

OBJS += \
./server2.o 

C_DEPS += \
./server2.d 


# Each subdirectory must supply rules for building sources it contributes
server2.o: /home/dushn/diplomski\ /making_chat_great_again/shot/server2.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


