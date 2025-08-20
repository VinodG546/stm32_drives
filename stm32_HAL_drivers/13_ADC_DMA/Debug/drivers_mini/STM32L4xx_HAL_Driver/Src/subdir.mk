################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.c \
../drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.c \
../drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.c \
../drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma_ex.c \
../drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_exti.c \
../drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.c \
../drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.c \
../drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.c \
../drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c \
../drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.c \
../drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.c \
../drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.c \
../drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.c 

OBJS += \
./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.o \
./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.o \
./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.o \
./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma_ex.o \
./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_exti.o \
./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.o \
./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.o \
./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.o \
./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.o \
./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.o \
./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.o \
./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.o \
./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.o 

C_DEPS += \
./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.d \
./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.d \
./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.d \
./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma_ex.d \
./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_exti.d \
./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.d \
./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.d \
./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.d \
./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.d \
./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.d \
./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.d \
./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.d \
./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.d 


# Each subdirectory must supply rules for building sources it contributes
drivers_mini/STM32L4xx_HAL_Driver/Src/%.o drivers_mini/STM32L4xx_HAL_Driver/Src/%.su drivers_mini/STM32L4xx_HAL_Driver/Src/%.cyclo: ../drivers_mini/STM32L4xx_HAL_Driver/Src/%.c drivers_mini/STM32L4xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32L4 -DSTM32 -DNUCLEO_L476RG -DSTM32L476RGTx -c -I../Inc -I"C:/Users/VINOD/STM32CubeIDE/workspace_1.19.0/02_g_pins/drivers_mini/CMSIS/Device/ST/STM32L4xx/Include" -I"C:/Users/VINOD/STM32CubeIDE/workspace_1.19.0/02_g_pins/drivers_mini/CMSIS/Include" -I"C:/Users/VINOD/STM32CubeIDE/workspace_1.19.0/02_g_pins/drivers_mini/STM32L4xx_HAL_Driver/Inc" -I"C:/Users/VINOD/STM32CubeIDE/workspace_1.19.0/02_g_pins/drivers_mini/STM32L4xx_HAL_Driver/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-drivers_mini-2f-STM32L4xx_HAL_Driver-2f-Src

clean-drivers_mini-2f-STM32L4xx_HAL_Driver-2f-Src:
	-$(RM) ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.cyclo ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.d ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.o ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.su ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.cyclo ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.d ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.o ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.su ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.cyclo ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.d ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.o ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.su ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma_ex.cyclo ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma_ex.d ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma_ex.o ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma_ex.su ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_exti.cyclo ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_exti.d ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_exti.o ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_exti.su ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.cyclo ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.d ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.o ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.su ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.cyclo ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.d ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.o ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.su ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.cyclo ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.d ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.o ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.su ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.cyclo ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.d ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.o ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.su ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.cyclo ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.d ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.o ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.su ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.cyclo ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.d ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.o ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.su ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.cyclo ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.d ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.o ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.su ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.cyclo ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.d ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.o ./drivers_mini/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.su

.PHONY: clean-drivers_mini-2f-STM32L4xx_HAL_Driver-2f-Src

