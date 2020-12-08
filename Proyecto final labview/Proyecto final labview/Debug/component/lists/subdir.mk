################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/lists/generic_list.c 

OBJS += \
./component/lists/generic_list.o 

C_DEPS += \
./component/lists/generic_list.d 


# Each subdirectory must supply rules for building sources it contributes
component/lists/%.o: ../component/lists/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_LPC54608J512ET180 -DCPU_LPC54608J512ET180_cm4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -I"D:\Users\salaz\Joe MCUXpresso\Proyecto final labview\Proyecto final labview\board" -I"D:\Users\salaz\Joe MCUXpresso\Proyecto final labview\Proyecto final labview\source" -I"D:\Users\salaz\Joe MCUXpresso\Proyecto final labview\Proyecto final labview" -I"D:\Users\salaz\Joe MCUXpresso\Proyecto final labview\Proyecto final labview\drivers" -I"D:\Users\salaz\Joe MCUXpresso\Proyecto final labview\Proyecto final labview\device" -I"D:\Users\salaz\Joe MCUXpresso\Proyecto final labview\Proyecto final labview\CMSIS" -I"D:\Users\salaz\Joe MCUXpresso\Proyecto final labview\Proyecto final labview\utilities" -I"D:\Users\salaz\Joe MCUXpresso\Proyecto final labview\Proyecto final labview\component\serial_manager" -I"D:\Users\salaz\Joe MCUXpresso\Proyecto final labview\Proyecto final labview\component\lists" -I"D:\Users\salaz\Joe MCUXpresso\Proyecto final labview\Proyecto final labview\component\uart" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__NEWLIB__ -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


