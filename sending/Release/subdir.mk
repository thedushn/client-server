################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/dushn/diplomski\ /making_chat_great_again/client.c \
/home/dushn/diplomski\ /making_chat_great_again/functions.c 

OBJS += \
./client.o \
./functions.o 

C_DEPS += \
./client.d \
./functions.d 


# Each subdirectory must supply rules for building sources it contributes
client.o: /home/dushn/diplomski\ /making_chat_great_again/client.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

functions.o: /home/dushn/diplomski\ /making_chat_great_again/functions.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


