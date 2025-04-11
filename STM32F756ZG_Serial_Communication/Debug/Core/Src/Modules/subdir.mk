################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Modules/command_parser.c \
../Core/Src/Modules/led_controller.c \
../Core/Src/Modules/module_wrappers.c \
../Core/Src/Modules/pattern_controller.c \
../Core/Src/Modules/timer_handler.c \
../Core/Src/Modules/uart_handler.c 

OBJS += \
./Core/Src/Modules/command_parser.o \
./Core/Src/Modules/led_controller.o \
./Core/Src/Modules/module_wrappers.o \
./Core/Src/Modules/pattern_controller.o \
./Core/Src/Modules/timer_handler.o \
./Core/Src/Modules/uart_handler.o 

C_DEPS += \
./Core/Src/Modules/command_parser.d \
./Core/Src/Modules/led_controller.d \
./Core/Src/Modules/module_wrappers.d \
./Core/Src/Modules/pattern_controller.d \
./Core/Src/Modules/timer_handler.d \
./Core/Src/Modules/uart_handler.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Modules/%.o Core/Src/Modules/%.su Core/Src/Modules/%.cyclo: ../Core/Src/Modules/%.c Core/Src/Modules/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F756xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Modules

clean-Core-2f-Src-2f-Modules:
	-$(RM) ./Core/Src/Modules/command_parser.cyclo ./Core/Src/Modules/command_parser.d ./Core/Src/Modules/command_parser.o ./Core/Src/Modules/command_parser.su ./Core/Src/Modules/led_controller.cyclo ./Core/Src/Modules/led_controller.d ./Core/Src/Modules/led_controller.o ./Core/Src/Modules/led_controller.su ./Core/Src/Modules/module_wrappers.cyclo ./Core/Src/Modules/module_wrappers.d ./Core/Src/Modules/module_wrappers.o ./Core/Src/Modules/module_wrappers.su ./Core/Src/Modules/pattern_controller.cyclo ./Core/Src/Modules/pattern_controller.d ./Core/Src/Modules/pattern_controller.o ./Core/Src/Modules/pattern_controller.su ./Core/Src/Modules/timer_handler.cyclo ./Core/Src/Modules/timer_handler.d ./Core/Src/Modules/timer_handler.o ./Core/Src/Modules/timer_handler.su ./Core/Src/Modules/uart_handler.cyclo ./Core/Src/Modules/uart_handler.d ./Core/Src/Modules/uart_handler.o ./Core/Src/Modules/uart_handler.su

.PHONY: clean-Core-2f-Src-2f-Modules

