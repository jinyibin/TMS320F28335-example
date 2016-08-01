################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../DSP2833x_Adc.c \
../DSP2833x_CpuTimers.c \
../DSP2833x_DMA.c \
../DSP2833x_DefaultIsr.c \
../DSP2833x_ECan.c \
../DSP2833x_GlobalVariableDefs.c \
../DSP2833x_MemCopy.c \
../DSP2833x_PieCtrl.c \
../DSP2833x_PieVect.c \
../DSP2833x_Sci.c \
../DSP2833x_SysCtrl.c \
../DSP2833x_Xintf.c \
../MotorCanCtrl.c \
../algrithom.c \
../maincontrol_dsp.c \
../uart_fpga.c 

ASM_SRCS += \
../DSP2833x_ADC_cal.asm \
../DSP2833x_CodeStartBranch.asm \
../DSP2833x_usDelay.asm 

CMD_SRCS += \
../28335_RAM_lnk.cmd \
../DSP2833x_Headers_nonBIOS.cmd 

ASM_DEPS += \
./DSP2833x_ADC_cal.pp \
./DSP2833x_CodeStartBranch.pp \
./DSP2833x_usDelay.pp 

OBJS += \
./DSP2833x_ADC_cal.obj \
./DSP2833x_Adc.obj \
./DSP2833x_CodeStartBranch.obj \
./DSP2833x_CpuTimers.obj \
./DSP2833x_DMA.obj \
./DSP2833x_DefaultIsr.obj \
./DSP2833x_ECan.obj \
./DSP2833x_GlobalVariableDefs.obj \
./DSP2833x_MemCopy.obj \
./DSP2833x_PieCtrl.obj \
./DSP2833x_PieVect.obj \
./DSP2833x_Sci.obj \
./DSP2833x_SysCtrl.obj \
./DSP2833x_Xintf.obj \
./DSP2833x_usDelay.obj \
./MotorCanCtrl.obj \
./algrithom.obj \
./maincontrol_dsp.obj \
./uart_fpga.obj 

C_DEPS += \
./DSP2833x_Adc.pp \
./DSP2833x_CpuTimers.pp \
./DSP2833x_DMA.pp \
./DSP2833x_DefaultIsr.pp \
./DSP2833x_ECan.pp \
./DSP2833x_GlobalVariableDefs.pp \
./DSP2833x_MemCopy.pp \
./DSP2833x_PieCtrl.pp \
./DSP2833x_PieVect.pp \
./DSP2833x_Sci.pp \
./DSP2833x_SysCtrl.pp \
./DSP2833x_Xintf.pp \
./MotorCanCtrl.pp \
./algrithom.pp \
./maincontrol_dsp.pp \
./uart_fpga.pp 

OBJS__QTD += \
".\DSP2833x_ADC_cal.obj" \
".\DSP2833x_Adc.obj" \
".\DSP2833x_CodeStartBranch.obj" \
".\DSP2833x_CpuTimers.obj" \
".\DSP2833x_DMA.obj" \
".\DSP2833x_DefaultIsr.obj" \
".\DSP2833x_ECan.obj" \
".\DSP2833x_GlobalVariableDefs.obj" \
".\DSP2833x_MemCopy.obj" \
".\DSP2833x_PieCtrl.obj" \
".\DSP2833x_PieVect.obj" \
".\DSP2833x_Sci.obj" \
".\DSP2833x_SysCtrl.obj" \
".\DSP2833x_Xintf.obj" \
".\DSP2833x_usDelay.obj" \
".\MotorCanCtrl.obj" \
".\algrithom.obj" \
".\maincontrol_dsp.obj" \
".\uart_fpga.obj" 

ASM_DEPS__QTD += \
".\DSP2833x_ADC_cal.pp" \
".\DSP2833x_CodeStartBranch.pp" \
".\DSP2833x_usDelay.pp" 

C_DEPS__QTD += \
".\DSP2833x_Adc.pp" \
".\DSP2833x_CpuTimers.pp" \
".\DSP2833x_DMA.pp" \
".\DSP2833x_DefaultIsr.pp" \
".\DSP2833x_ECan.pp" \
".\DSP2833x_GlobalVariableDefs.pp" \
".\DSP2833x_MemCopy.pp" \
".\DSP2833x_PieCtrl.pp" \
".\DSP2833x_PieVect.pp" \
".\DSP2833x_Sci.pp" \
".\DSP2833x_SysCtrl.pp" \
".\DSP2833x_Xintf.pp" \
".\MotorCanCtrl.pp" \
".\algrithom.pp" \
".\maincontrol_dsp.pp" \
".\uart_fpga.pp" 

ASM_SRCS_QUOTED += \
"../DSP2833x_ADC_cal.asm" \
"../DSP2833x_CodeStartBranch.asm" \
"../DSP2833x_usDelay.asm" 

C_SRCS_QUOTED += \
"../DSP2833x_Adc.c" \
"../DSP2833x_CpuTimers.c" \
"../DSP2833x_DMA.c" \
"../DSP2833x_DefaultIsr.c" \
"../DSP2833x_ECan.c" \
"../DSP2833x_GlobalVariableDefs.c" \
"../DSP2833x_MemCopy.c" \
"../DSP2833x_PieCtrl.c" \
"../DSP2833x_PieVect.c" \
"../DSP2833x_Sci.c" \
"../DSP2833x_SysCtrl.c" \
"../DSP2833x_Xintf.c" \
"../MotorCanCtrl.c" \
"../algrithom.c" \
"../maincontrol_dsp.c" \
"../uart_fpga.c" 


# Each subdirectory must supply rules for building sources it contributes
DSP2833x_ADC_cal.obj: ../DSP2833x_ADC_cal.asm $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/bin/cl2000" --silicon_version=28 -g -O2 --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/include" --include_path="E:/CCS_workspace/maincontrol_dsp/include" --diag_warning=225 --large_memory_model --float_support=fpu32 --preproc_with_compile --preproc_dependency="DSP2833x_ADC_cal.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

DSP2833x_Adc.obj: ../DSP2833x_Adc.c $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/bin/cl2000" --silicon_version=28 -g -O2 --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/include" --include_path="E:/CCS_workspace/maincontrol_dsp/include" --diag_warning=225 --large_memory_model --float_support=fpu32 --preproc_with_compile --preproc_dependency="DSP2833x_Adc.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

DSP2833x_CodeStartBranch.obj: ../DSP2833x_CodeStartBranch.asm $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/bin/cl2000" --silicon_version=28 -g -O2 --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/include" --include_path="E:/CCS_workspace/maincontrol_dsp/include" --diag_warning=225 --large_memory_model --float_support=fpu32 --preproc_with_compile --preproc_dependency="DSP2833x_CodeStartBranch.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

DSP2833x_CpuTimers.obj: ../DSP2833x_CpuTimers.c $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/bin/cl2000" --silicon_version=28 -g -O2 --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/include" --include_path="E:/CCS_workspace/maincontrol_dsp/include" --diag_warning=225 --large_memory_model --float_support=fpu32 --preproc_with_compile --preproc_dependency="DSP2833x_CpuTimers.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

DSP2833x_DMA.obj: ../DSP2833x_DMA.c $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/bin/cl2000" --silicon_version=28 -g -O2 --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/include" --include_path="E:/CCS_workspace/maincontrol_dsp/include" --diag_warning=225 --large_memory_model --float_support=fpu32 --preproc_with_compile --preproc_dependency="DSP2833x_DMA.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

DSP2833x_DefaultIsr.obj: ../DSP2833x_DefaultIsr.c $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/bin/cl2000" --silicon_version=28 -g -O2 --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/include" --include_path="E:/CCS_workspace/maincontrol_dsp/include" --diag_warning=225 --large_memory_model --float_support=fpu32 --preproc_with_compile --preproc_dependency="DSP2833x_DefaultIsr.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

DSP2833x_ECan.obj: ../DSP2833x_ECan.c $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/bin/cl2000" --silicon_version=28 -g -O2 --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/include" --include_path="E:/CCS_workspace/maincontrol_dsp/include" --diag_warning=225 --large_memory_model --float_support=fpu32 --preproc_with_compile --preproc_dependency="DSP2833x_ECan.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

DSP2833x_GlobalVariableDefs.obj: ../DSP2833x_GlobalVariableDefs.c $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/bin/cl2000" --silicon_version=28 -g -O2 --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/include" --include_path="E:/CCS_workspace/maincontrol_dsp/include" --diag_warning=225 --large_memory_model --float_support=fpu32 --preproc_with_compile --preproc_dependency="DSP2833x_GlobalVariableDefs.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

DSP2833x_MemCopy.obj: ../DSP2833x_MemCopy.c $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/bin/cl2000" --silicon_version=28 -g -O2 --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/include" --include_path="E:/CCS_workspace/maincontrol_dsp/include" --diag_warning=225 --large_memory_model --float_support=fpu32 --preproc_with_compile --preproc_dependency="DSP2833x_MemCopy.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

DSP2833x_PieCtrl.obj: ../DSP2833x_PieCtrl.c $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/bin/cl2000" --silicon_version=28 -g -O2 --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/include" --include_path="E:/CCS_workspace/maincontrol_dsp/include" --diag_warning=225 --large_memory_model --float_support=fpu32 --preproc_with_compile --preproc_dependency="DSP2833x_PieCtrl.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

DSP2833x_PieVect.obj: ../DSP2833x_PieVect.c $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/bin/cl2000" --silicon_version=28 -g -O2 --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/include" --include_path="E:/CCS_workspace/maincontrol_dsp/include" --diag_warning=225 --large_memory_model --float_support=fpu32 --preproc_with_compile --preproc_dependency="DSP2833x_PieVect.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

DSP2833x_Sci.obj: ../DSP2833x_Sci.c $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/bin/cl2000" --silicon_version=28 -g -O2 --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/include" --include_path="E:/CCS_workspace/maincontrol_dsp/include" --diag_warning=225 --large_memory_model --float_support=fpu32 --preproc_with_compile --preproc_dependency="DSP2833x_Sci.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

DSP2833x_SysCtrl.obj: ../DSP2833x_SysCtrl.c $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/bin/cl2000" --silicon_version=28 -g -O2 --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/include" --include_path="E:/CCS_workspace/maincontrol_dsp/include" --diag_warning=225 --large_memory_model --float_support=fpu32 --preproc_with_compile --preproc_dependency="DSP2833x_SysCtrl.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

DSP2833x_Xintf.obj: ../DSP2833x_Xintf.c $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/bin/cl2000" --silicon_version=28 -g -O2 --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/include" --include_path="E:/CCS_workspace/maincontrol_dsp/include" --diag_warning=225 --large_memory_model --float_support=fpu32 --preproc_with_compile --preproc_dependency="DSP2833x_Xintf.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

DSP2833x_usDelay.obj: ../DSP2833x_usDelay.asm $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/bin/cl2000" --silicon_version=28 -g -O2 --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/include" --include_path="E:/CCS_workspace/maincontrol_dsp/include" --diag_warning=225 --large_memory_model --float_support=fpu32 --preproc_with_compile --preproc_dependency="DSP2833x_usDelay.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

MotorCanCtrl.obj: ../MotorCanCtrl.c $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/bin/cl2000" --silicon_version=28 -g -O2 --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/include" --include_path="E:/CCS_workspace/maincontrol_dsp/include" --diag_warning=225 --large_memory_model --float_support=fpu32 --preproc_with_compile --preproc_dependency="MotorCanCtrl.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

algrithom.obj: ../algrithom.c $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/bin/cl2000" --silicon_version=28 -g -O2 --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/include" --include_path="E:/CCS_workspace/maincontrol_dsp/include" --diag_warning=225 --large_memory_model --float_support=fpu32 --preproc_with_compile --preproc_dependency="algrithom.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

maincontrol_dsp.obj: ../maincontrol_dsp.c $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/bin/cl2000" --silicon_version=28 -g -O2 --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/include" --include_path="E:/CCS_workspace/maincontrol_dsp/include" --diag_warning=225 --large_memory_model --float_support=fpu32 --preproc_with_compile --preproc_dependency="maincontrol_dsp.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

uart_fpga.obj: ../uart_fpga.c $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/bin/cl2000" --silicon_version=28 -g -O2 --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/include" --include_path="E:/CCS_workspace/maincontrol_dsp/include" --diag_warning=225 --large_memory_model --float_support=fpu32 --preproc_with_compile --preproc_dependency="uart_fpga.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '


