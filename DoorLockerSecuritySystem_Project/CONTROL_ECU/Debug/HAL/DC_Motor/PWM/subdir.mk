################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HAL/DC_Motor/PWM/pwm_timer0.c 

OBJS += \
./HAL/DC_Motor/PWM/pwm_timer0.o 

C_DEPS += \
./HAL/DC_Motor/PWM/pwm_timer0.d 


# Each subdirectory must supply rules for building sources it contributes
HAL/DC_Motor/PWM/%.o: ../HAL/DC_Motor/PWM/%.c HAL/DC_Motor/PWM/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


