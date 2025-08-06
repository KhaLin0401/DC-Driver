################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/ModbusMap.c \
../Core/Src/MotorDC.c \
../Core/Src/PID.c \
../Core/Src/SystemStatus.c \
../Core/Src/freertos.c \
../Core/Src/main.c \
../Core/Src/modbus.c \
../Core/Src/modbus_crc.c \
../Core/Src/modbus_port.c \
../Core/Src/stm32f1xx_hal_msp.c \
../Core/Src/stm32f1xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f1xx.c 

OBJS += \
./Core/Src/ModbusMap.o \
./Core/Src/MotorDC.o \
./Core/Src/PID.o \
./Core/Src/SystemStatus.o \
./Core/Src/freertos.o \
./Core/Src/main.o \
./Core/Src/modbus.o \
./Core/Src/modbus_crc.o \
./Core/Src/modbus_port.o \
./Core/Src/stm32f1xx_hal_msp.o \
./Core/Src/stm32f1xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f1xx.o 

C_DEPS += \
./Core/Src/ModbusMap.d \
./Core/Src/MotorDC.d \
./Core/Src/PID.d \
./Core/Src/SystemStatus.d \
./Core/Src/freertos.d \
./Core/Src/main.d \
./Core/Src/modbus.d \
./Core/Src/modbus_crc.d \
./Core/Src/modbus_port.d \
./Core/Src/stm32f1xx_hal_msp.d \
./Core/Src/stm32f1xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I"C:/Users/ASUS/Desktop/RAYBOT/SOURCE/DC-Driver/Code/modbus" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/ModbusMap.cyclo ./Core/Src/ModbusMap.d ./Core/Src/ModbusMap.o ./Core/Src/ModbusMap.su ./Core/Src/MotorDC.cyclo ./Core/Src/MotorDC.d ./Core/Src/MotorDC.o ./Core/Src/MotorDC.su ./Core/Src/PID.cyclo ./Core/Src/PID.d ./Core/Src/PID.o ./Core/Src/PID.su ./Core/Src/SystemStatus.cyclo ./Core/Src/SystemStatus.d ./Core/Src/SystemStatus.o ./Core/Src/SystemStatus.su ./Core/Src/freertos.cyclo ./Core/Src/freertos.d ./Core/Src/freertos.o ./Core/Src/freertos.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/modbus.cyclo ./Core/Src/modbus.d ./Core/Src/modbus.o ./Core/Src/modbus.su ./Core/Src/modbus_crc.cyclo ./Core/Src/modbus_crc.d ./Core/Src/modbus_crc.o ./Core/Src/modbus_crc.su ./Core/Src/modbus_port.cyclo ./Core/Src/modbus_port.d ./Core/Src/modbus_port.o ./Core/Src/modbus_port.su ./Core/Src/stm32f1xx_hal_msp.cyclo ./Core/Src/stm32f1xx_hal_msp.d ./Core/Src/stm32f1xx_hal_msp.o ./Core/Src/stm32f1xx_hal_msp.su ./Core/Src/stm32f1xx_it.cyclo ./Core/Src/stm32f1xx_it.d ./Core/Src/stm32f1xx_it.o ./Core/Src/stm32f1xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f1xx.cyclo ./Core/Src/system_stm32f1xx.d ./Core/Src/system_stm32f1xx.o ./Core/Src/system_stm32f1xx.su

.PHONY: clean-Core-2f-Src

