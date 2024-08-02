################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Peri/TM1650/%.o: ../Peri/TM1650/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/ti/ccs1260/ccs/tools/compiler/ti-cgt-armllvm_3.2.1.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/RATTAN/Desktop/ccs workspace/Car Ver5.0/Driver" -I"C:/Users/RATTAN/Desktop/ccs workspace/Car Ver5.0" -I"C:/Users/RATTAN/Desktop/ccs workspace/Car Ver5.0/Debug" -I"C:/ti/mspm0_sdk_2_01_00_03/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_01_00_03/source" -I"C:/Users/RATTAN/Desktop/ccs workspace/Car Ver5.0/Peri/OLED" -I"C:/Users/RATTAN/Desktop/ccs workspace/Car Ver5.0/Peri/photocell8bits" -I"C:/Users/RATTAN/Desktop/ccs workspace/Car Ver5.0/APP" -I"C:/Users/RATTAN/Desktop/ccs workspace/Car Ver5.0/Peri/TM1650" -gdwarf-3 -MMD -MP -MF"Peri/TM1650/$(basename $(<F)).d_raw" -MT"$(@)" -std=c11 $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


