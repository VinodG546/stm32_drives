################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/src/gpio_driver.c \
../drivers/src/spi_driver.c 

OBJS += \
./drivers/src/gpio_driver.o \
./drivers/src/spi_driver.o 

C_DEPS += \
./drivers/src/gpio_driver.d \
./drivers/src/spi_driver.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/src/%.o drivers/src/%.su drivers/src/%.cyclo: ../drivers/src/%.c drivers/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32L4 -DSTM32 -DNUCLEO_L476RG -DSTM32L476RGTx -c -I"C:/Users/VINOD/STM32CubeIDE/stm32_low_level_drivers/new_drivers/drivers/inc" -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-drivers-2f-src

clean-drivers-2f-src:
	-$(RM) ./drivers/src/gpio_driver.cyclo ./drivers/src/gpio_driver.d ./drivers/src/gpio_driver.o ./drivers/src/gpio_driver.su ./drivers/src/spi_driver.cyclo ./drivers/src/spi_driver.d ./drivers/src/spi_driver.o ./drivers/src/spi_driver.su

.PHONY: clean-drivers-2f-src

