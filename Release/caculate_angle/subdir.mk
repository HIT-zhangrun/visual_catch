################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../caculate_angle/caculate_angle.c 

OBJS += \
./caculate_angle/caculate_angle.o 

C_DEPS += \
./caculate_angle/caculate_angle.d 


# Each subdirectory must supply rules for building sources it contributes
caculate_angle/%.o caculate_angle/%.su: ../caculate_angle/%.c caculate_angle/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../caculate_angle -IC:/Users/zhangrun/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Inc -IC:/Users/zhangrun/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -IC:/Users/zhangrun/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.4/Drivers/CMSIS/Device/ST/STM32F1xx/Include -IC:/Users/zhangrun/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.4/Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -IC:/Users/zhangrun/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -IC:/Users/zhangrun/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-caculate_angle

clean-caculate_angle:
	-$(RM) ./caculate_angle/caculate_angle.d ./caculate_angle/caculate_angle.o ./caculate_angle/caculate_angle.su

.PHONY: clean-caculate_angle

