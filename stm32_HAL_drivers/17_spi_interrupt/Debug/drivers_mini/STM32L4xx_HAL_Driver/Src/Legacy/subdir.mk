################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers_mini/STM32L4xx_HAL_Driver/Src/Legacy/stm32l4xx_hal_can.c 

OBJS += \
./drivers_mini/STM32L4xx_HAL_Driver/Src/Legacy/stm32l4xx_hal_can.o 

C_DEPS += \
./drivers_mini/STM32L4xx_HAL_Driver/Src/Legacy/stm32l4xx_hal_can.d 


# Each subdirectory must supply rules for building sources it contributes
drivers_mini/STM32L4xx_HAL_Driver/Src/Legacy/%.o drivers_mini/STM32L4xx_HAL_Driver/Src/Legacy/%.su drivers_mini/STM32L4xx_HAL_Driver/Src/Legacy/%.cyclo: ../drivers_mini/STM32L4xx_HAL_Driver/Src/Legacy/%.c drivers_mini/STM32L4xx_HAL_Driver/Src/Legacy/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32L4 -DSTM32 -DNUCLEO_L476RG -DSTM32L476RGTx -c -I../Inc -I"C:/Users/VINOD/STM32CubeIDE/workspace_1.19.0/02_g_pins/drivers_mini/CMSIS/Device/ST/STM32L4xx/Include" -I"C:/Users/VINOD/STM32CubeIDE/workspace_1.19.0/02_g_pins/drivers_mini/CMSIS/Include" -I"C:/Users/VINOD/STM32CubeIDE/workspace_1.19.0/02_g_pins/drivers_mini/STM32L4xx_HAL_Driver/Inc" -I"C:/Users/VINOD/STM32CubeIDE/workspace_1.19.0/02_g_pins/drivers_mini/STM32L4xx_HAL_Driver/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-drivers_mini-2f-STM32L4xx_HAL_Driver-2f-Src-2f-Legacy

clean-drivers_mini-2f-STM32L4xx_HAL_Driver-2f-Src-2f-Legacy:
	-$(RM) ./drivers_mini/STM32L4xx_HAL_Driver/Src/Legacy/stm32l4xx_hal_can.cyclo ./drivers_mini/STM32L4xx_HAL_Driver/Src/Legacy/stm32l4xx_hal_can.d ./drivers_mini/STM32L4xx_HAL_Driver/Src/Legacy/stm32l4xx_hal_can.o ./drivers_mini/STM32L4xx_HAL_Driver/Src/Legacy/stm32l4xx_hal_can.su

.PHONY: clean-drivers_mini-2f-STM32L4xx_HAL_Driver-2f-Src-2f-Legacy

