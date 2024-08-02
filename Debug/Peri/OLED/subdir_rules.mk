################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Peri/OLED/%.o: ../Peri/OLED/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/ti/ccs1260/ccs/tools/compiler/ti-cgt-armllvm_3.2.1.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/RATTAN/Desktop/ccs workspace/Car Ver6.0/Driver" -I"C:/Users/RATTAN/Desktop/ccs workspace/Car Ver6.0" -I"C:/Users/RATTAN/Desktop/ccs workspace/Car Ver6.0/Debug" -I"C:/ti/mspm0_sdk_2_01_00_03/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_01_00_03/source" -I"C:/Users/RATTAN/Desktop/ccs workspace/Car Ver6.0/Peri/OLED" -I"C:/Users/RATTAN/Desktop/ccs workspace/Car Ver6.0/Peri/photocell8bits" -I"C:/Users/RATTAN/Desktop/ccs workspace/Car Ver6.0/APP" -gdwarf-3 -MMD -MP -MF"Peri/OLED/$(basename $(<F)).d_raw" -MT"$(@)" -std=c11 $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


