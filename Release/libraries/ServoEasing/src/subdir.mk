################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
..\libraries\ServoEasing\src\LightweightServo.cpp \
..\libraries\ServoEasing\src\ServoEasing.cpp 

LINK_OBJ += \
.\libraries\ServoEasing\src\LightweightServo.cpp.o \
.\libraries\ServoEasing\src\ServoEasing.cpp.o 

CPP_DEPS += \
.\libraries\ServoEasing\src\LightweightServo.cpp.d \
.\libraries\ServoEasing\src\ServoEasing.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
libraries\ServoEasing\src\LightweightServo.cpp.o: C:\Users\Alexa\Documents\Sloeber\Spider\libraries\ServoEasing\src\LightweightServo.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Sloeber\arduinoPlugin\packages\arduino\tools\avr-gcc\7.3.0-atmel3.6.1-arduino5/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_NANO -DARDUINO_ARCH_AVR     -I"C:\Sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.8.2\cores\arduino" -I"C:\Sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.8.2\variants\eightanaloginputs" -I"C:\Users\Alexa\Documents\Sloeber\Spider\libraries\Coordinates\src" -I"C:\Users\Alexa\Documents\Sloeber\Spider\libraries\ServoEasing\src" -I"C:\Users\Alexa\Documents\Sloeber\Spider\libraries\PlotPlus" -I"C:\Sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.8.2\libraries\Wire\src" -I"C:\Users\Alexa\Documents\Sloeber\Spider\libraries\Spider" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\ServoEasing\src\ServoEasing.cpp.o: C:\Users\Alexa\Documents\Sloeber\Spider\libraries\ServoEasing\src\ServoEasing.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Sloeber\arduinoPlugin\packages\arduino\tools\avr-gcc\7.3.0-atmel3.6.1-arduino5/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_NANO -DARDUINO_ARCH_AVR     -I"C:\Sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.8.2\cores\arduino" -I"C:\Sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.8.2\variants\eightanaloginputs" -I"C:\Users\Alexa\Documents\Sloeber\Spider\libraries\Coordinates\src" -I"C:\Users\Alexa\Documents\Sloeber\Spider\libraries\ServoEasing\src" -I"C:\Users\Alexa\Documents\Sloeber\Spider\libraries\PlotPlus" -I"C:\Sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.8.2\libraries\Wire\src" -I"C:\Users\Alexa\Documents\Sloeber\Spider\libraries\Spider" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


