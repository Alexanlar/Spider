################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:\Sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.8.2\libraries\Wire\src\utility\twi.c 

C_DEPS += \
.\libraries\Wire\src\utility\twi.c.d 

LINK_OBJ += \
.\libraries\Wire\src\utility\twi.c.o 


# Each subdirectory must supply rules for building sources it contributes
libraries\Wire\src\utility\twi.c.o: C:\Sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.8.2\libraries\Wire\src\utility\twi.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"C:\Sloeber\arduinoPlugin\packages\arduino\tools\avr-gcc\7.3.0-atmel3.6.1-arduino5/bin/avr-gcc" -c -g -Os -Wall -Wextra -std=gnu11 -ffunction-sections -fdata-sections -MMD -flto -fno-fat-lto-objects -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_NANO -DARDUINO_ARCH_AVR     -I"C:\Sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.8.2\cores\arduino" -I"C:\Sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.8.2\variants\eightanaloginputs" -I"C:\Users\Alexa\Documents\Arduino\libraries\Spider" -I"C:\Users\Alexa\Documents\Arduino\libraries\Coordinates\src" -I"C:\Users\Alexa\Documents\Arduino\libraries\ServoEasing\src" -I"C:\Sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.8.2\libraries\Wire\src" -I"C:\Users\Alexa\Documents\Arduino\libraries\PlotPlus" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


