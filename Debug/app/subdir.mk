################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/app.c \
../app/buzzer.c \
../app/hw.c \
../app/lcd.c 

OBJS += \
./app/app.o \
./app/buzzer.o \
./app/hw.o \
./app/lcd.o 

C_DEPS += \
./app/app.d \
./app/buzzer.d \
./app/hw.d \
./app/lcd.d 


# Each subdirectory must supply rules for building sources it contributes
app/%.o app/%.su app/%.cyclo: ../app/%.c app/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../app -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-app

clean-app:
	-$(RM) ./app/app.cyclo ./app/app.d ./app/app.o ./app/app.su ./app/buzzer.cyclo ./app/buzzer.d ./app/buzzer.o ./app/buzzer.su ./app/hw.cyclo ./app/hw.d ./app/hw.o ./app/hw.su ./app/lcd.cyclo ./app/lcd.d ./app/lcd.o ./app/lcd.su

.PHONY: clean-app

