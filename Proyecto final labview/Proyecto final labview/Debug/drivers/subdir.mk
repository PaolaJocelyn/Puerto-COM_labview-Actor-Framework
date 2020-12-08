################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_adc.c \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_emc.c \
../drivers/fsl_flexcomm.c \
../drivers/fsl_gpio.c \
../drivers/fsl_i2c.c \
../drivers/fsl_inputmux.c \
../drivers/fsl_pint.c \
../drivers/fsl_power.c \
../drivers/fsl_reset.c \
../drivers/fsl_spi.c \
../drivers/fsl_usart.c 

OBJS += \
./drivers/fsl_adc.o \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_emc.o \
./drivers/fsl_flexcomm.o \
./drivers/fsl_gpio.o \
./drivers/fsl_i2c.o \
./drivers/fsl_inputmux.o \
./drivers/fsl_pint.o \
./drivers/fsl_power.o \
./drivers/fsl_reset.o \
./drivers/fsl_spi.o \
./drivers/fsl_usart.o 

C_DEPS += \
./drivers/fsl_adc.d \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_emc.d \
./drivers/fsl_flexcomm.d \
./drivers/fsl_gpio.d \
./drivers/fsl_i2c.d \
./drivers/fsl_inputmux.d \
./drivers/fsl_pint.d \
./drivers/fsl_power.d \
./drivers/fsl_reset.d \
./drivers/fsl_spi.d \
./drivers/fsl_usart.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_LPC54608J512ET180 -DCPU_LPC54608J512ET180_cm4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -I"D:\Users\salaz\Joe MCUXpresso\Proyecto final labview\Proyecto final labview\board" -I"D:\Users\salaz\Joe MCUXpresso\Proyecto final labview\Proyecto final labview\source" -I"D:\Users\salaz\Joe MCUXpresso\Proyecto final labview\Proyecto final labview" -I"D:\Users\salaz\Joe MCUXpresso\Proyecto final labview\Proyecto final labview\drivers" -I"D:\Users\salaz\Joe MCUXpresso\Proyecto final labview\Proyecto final labview\device" -I"D:\Users\salaz\Joe MCUXpresso\Proyecto final labview\Proyecto final labview\CMSIS" -I"D:\Users\salaz\Joe MCUXpresso\Proyecto final labview\Proyecto final labview\utilities" -I"D:\Users\salaz\Joe MCUXpresso\Proyecto final labview\Proyecto final labview\component\serial_manager" -I"D:\Users\salaz\Joe MCUXpresso\Proyecto final labview\Proyecto final labview\component\lists" -I"D:\Users\salaz\Joe MCUXpresso\Proyecto final labview\Proyecto final labview\component\uart" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__NEWLIB__ -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


