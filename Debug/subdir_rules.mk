################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
DSP2833x_ADC_cal.obj: ../DSP2833x_ADC_cal.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -Ooff --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_headers/include" --include_path="C:/ti/controlSUITE/libs/math/IQmath/v15c/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_common/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_headers/source" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2833x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/motor_control/math_blocks/v4.3" --include_path="C:/ti/controlSUITE/libs/app_libs/motor_control/drivers/f2833x_v2.0" --include_path="C:/Users/Ujval/Documents/Sameer/Code/DDP_Motor" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="DSP2833x_ADC_cal.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

DSP2833x_CodeStartBranch.obj: C:/ti/C2000Ware_1_00_06_00_Software/device_support/f2833x/common/source/DSP2833x_CodeStartBranch.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -Ooff --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_headers/include" --include_path="C:/ti/controlSUITE/libs/math/IQmath/v15c/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_common/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_headers/source" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2833x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/motor_control/math_blocks/v4.3" --include_path="C:/ti/controlSUITE/libs/app_libs/motor_control/drivers/f2833x_v2.0" --include_path="C:/Users/Ujval/Documents/Sameer/Code/DDP_Motor" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="DSP2833x_CodeStartBranch.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

DSP2833x_DefaultIsr.obj: C:/ti/C2000Ware_1_00_06_00_Software/device_support/f2833x/common/source/DSP2833x_DefaultIsr.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -Ooff --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_headers/include" --include_path="C:/ti/controlSUITE/libs/math/IQmath/v15c/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_common/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_headers/source" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2833x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/motor_control/math_blocks/v4.3" --include_path="C:/ti/controlSUITE/libs/app_libs/motor_control/drivers/f2833x_v2.0" --include_path="C:/Users/Ujval/Documents/Sameer/Code/DDP_Motor" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="DSP2833x_DefaultIsr.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

DSP2833x_EPwm.obj: ../DSP2833x_EPwm.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -Ooff --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_headers/include" --include_path="C:/ti/controlSUITE/libs/math/IQmath/v15c/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_common/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_headers/source" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2833x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/motor_control/math_blocks/v4.3" --include_path="C:/ti/controlSUITE/libs/app_libs/motor_control/drivers/f2833x_v2.0" --include_path="C:/Users/Ujval/Documents/Sameer/Code/DDP_Motor" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="DSP2833x_EPwm.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

DSP2833x_GlobalVariableDefs.obj: C:/ti/C2000Ware_1_00_06_00_Software/device_support/f2833x/headers/source/DSP2833x_GlobalVariableDefs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -Ooff --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_headers/include" --include_path="C:/ti/controlSUITE/libs/math/IQmath/v15c/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_common/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_headers/source" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2833x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/motor_control/math_blocks/v4.3" --include_path="C:/ti/controlSUITE/libs/app_libs/motor_control/drivers/f2833x_v2.0" --include_path="C:/Users/Ujval/Documents/Sameer/Code/DDP_Motor" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="DSP2833x_GlobalVariableDefs.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

DSP2833x_PieCtrl.obj: C:/ti/C2000Ware_1_00_06_00_Software/device_support/f2833x/common/source/DSP2833x_PieCtrl.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -Ooff --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_headers/include" --include_path="C:/ti/controlSUITE/libs/math/IQmath/v15c/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_common/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_headers/source" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2833x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/motor_control/math_blocks/v4.3" --include_path="C:/ti/controlSUITE/libs/app_libs/motor_control/drivers/f2833x_v2.0" --include_path="C:/Users/Ujval/Documents/Sameer/Code/DDP_Motor" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="DSP2833x_PieCtrl.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

DSP2833x_PieVect.obj: C:/ti/C2000Ware_1_00_06_00_Software/device_support/f2833x/common/source/DSP2833x_PieVect.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -Ooff --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_headers/include" --include_path="C:/ti/controlSUITE/libs/math/IQmath/v15c/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_common/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_headers/source" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2833x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/motor_control/math_blocks/v4.3" --include_path="C:/ti/controlSUITE/libs/app_libs/motor_control/drivers/f2833x_v2.0" --include_path="C:/Users/Ujval/Documents/Sameer/Code/DDP_Motor" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="DSP2833x_PieVect.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

DSP2833x_SysCtrl.obj: C:/ti/C2000Ware_1_00_06_00_Software/device_support/f2833x/common/source/DSP2833x_SysCtrl.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -Ooff --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_headers/include" --include_path="C:/ti/controlSUITE/libs/math/IQmath/v15c/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_common/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_headers/source" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2833x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/motor_control/math_blocks/v4.3" --include_path="C:/ti/controlSUITE/libs/app_libs/motor_control/drivers/f2833x_v2.0" --include_path="C:/Users/Ujval/Documents/Sameer/Code/DDP_Motor" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="DSP2833x_SysCtrl.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

DSP2833x_usDelay.obj: C:/ti/C2000Ware_1_00_06_00_Software/device_support/f2833x/common/source/DSP2833x_usDelay.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -Ooff --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_headers/include" --include_path="C:/ti/controlSUITE/libs/math/IQmath/v15c/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_common/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_headers/source" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2833x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/motor_control/math_blocks/v4.3" --include_path="C:/ti/controlSUITE/libs/app_libs/motor_control/drivers/f2833x_v2.0" --include_path="C:/Users/Ujval/Documents/Sameer/Code/DDP_Motor" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="DSP2833x_usDelay.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Gpio_encoder.obj: ../Gpio_encoder.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -Ooff --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_headers/include" --include_path="C:/ti/controlSUITE/libs/math/IQmath/v15c/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_common/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_headers/source" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2833x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/motor_control/math_blocks/v4.3" --include_path="C:/ti/controlSUITE/libs/app_libs/motor_control/drivers/f2833x_v2.0" --include_path="C:/Users/Ujval/Documents/Sameer/Code/DDP_Motor" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Gpio_encoder.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

ekf_cal.obj: ../ekf_cal.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -Ooff --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_headers/include" --include_path="C:/ti/controlSUITE/libs/math/IQmath/v15c/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_common/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_headers/source" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2833x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/motor_control/math_blocks/v4.3" --include_path="C:/ti/controlSUITE/libs/app_libs/motor_control/drivers/f2833x_v2.0" --include_path="C:/Users/Ujval/Documents/Sameer/Code/DDP_Motor" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="ekf_cal.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -Ooff --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_headers/include" --include_path="C:/ti/controlSUITE/libs/math/IQmath/v15c/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_common/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v142/DSP2833x_headers/source" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2833x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/motor_control/math_blocks/v4.3" --include_path="C:/ti/controlSUITE/libs/app_libs/motor_control/drivers/f2833x_v2.0" --include_path="C:/Users/Ujval/Documents/Sameer/Code/DDP_Motor" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/include" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="main.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


