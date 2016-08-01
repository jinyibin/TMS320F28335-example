################################################################################
# Automatically-generated file. Do not edit!
################################################################################

EMPTY := 
SPACE := $(EMPTY) $(EMPTY)

ORDERED_OBJS += \
$(GEN_CMDS_QUOTED) \
"./uart_fpga.obj" \
"./maincontrol_dsp.obj" \
"./algrithom.obj" \
"./MotorCanCtrl.obj" \
"./DSP2833x_usDelay.obj" \
"./DSP2833x_Xintf.obj" \
"./DSP2833x_SysCtrl.obj" \
"./DSP2833x_Sci.obj" \
"./DSP2833x_PieVect.obj" \
"./DSP2833x_PieCtrl.obj" \
"./DSP2833x_MemCopy.obj" \
"./DSP2833x_GlobalVariableDefs.obj" \
"./DSP2833x_ECan.obj" \
"./DSP2833x_DefaultIsr.obj" \
"./DSP2833x_DMA.obj" \
"./DSP2833x_CpuTimers.obj" \
"./DSP2833x_CodeStartBranch.obj" \
"./DSP2833x_Adc.obj" \
"./DSP2833x_ADC_cal.obj" \
-l"libc.a" \
-l"rts2800_fpu32.lib" \
"../28335_RAM_lnk.cmd" \
"../DSP2833x_Headers_nonBIOS.cmd" \

-include ../makefile.init

RM := DEL /F

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: maincontrol_dsp.out

# Tool invocations
maincontrol_dsp.out: $(GEN_CMDS) $(CMD_SRCS) $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Linker'
	$(shell echo --silicon_version=28 > ccsLinker.opt)
	$(shell echo -g >> ccsLinker.opt)
	$(shell echo --diag_warning=225 >> ccsLinker.opt)
	$(shell echo --large_memory_model >> ccsLinker.opt)
	$(shell echo --float_support=fpu32 >> ccsLinker.opt)
	$(shell echo -z >> ccsLinker.opt)
	$(shell echo -m"maincontrol_dsp.map" >> ccsLinker.opt)
	$(shell echo --stack_size=0x380 >> ccsLinker.opt)
	$(shell echo --warn_sections >> ccsLinker.opt)
	$(shell echo -i"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/lib" -i"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/include" >> ccsLinker.opt)
	$(shell echo --reread_libs >> ccsLinker.opt)
	$(shell echo --rom_model >> ccsLinker.opt)
	$(shell type ccsObjs.opt >> ccsLinker.opt)
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/c2000/bin/cl2000" -@ccsLinker.opt -o "maincontrol_dsp.out"
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C2000_EXECUTABLE_OUTPUTS__QTD) "maincontrol_dsp.out"
	-$(RM) ".\DSP2833x_ADC_cal.obj" ".\DSP2833x_Adc.obj" ".\DSP2833x_CodeStartBranch.obj" ".\DSP2833x_CpuTimers.obj" ".\DSP2833x_DMA.obj" ".\DSP2833x_DefaultIsr.obj" ".\DSP2833x_ECan.obj" ".\DSP2833x_GlobalVariableDefs.obj" ".\DSP2833x_MemCopy.obj" ".\DSP2833x_PieCtrl.obj" ".\DSP2833x_PieVect.obj" ".\DSP2833x_Sci.obj" ".\DSP2833x_SysCtrl.obj" ".\DSP2833x_Xintf.obj" ".\DSP2833x_usDelay.obj" ".\MotorCanCtrl.obj" ".\algrithom.obj" ".\maincontrol_dsp.obj" ".\uart_fpga.obj" 
	-$(RM) ".\DSP2833x_ADC_cal.pp" ".\DSP2833x_CodeStartBranch.pp" ".\DSP2833x_usDelay.pp" 
	-$(RM) ".\DSP2833x_Adc.pp" ".\DSP2833x_CpuTimers.pp" ".\DSP2833x_DMA.pp" ".\DSP2833x_DefaultIsr.pp" ".\DSP2833x_ECan.pp" ".\DSP2833x_GlobalVariableDefs.pp" ".\DSP2833x_MemCopy.pp" ".\DSP2833x_PieCtrl.pp" ".\DSP2833x_PieVect.pp" ".\DSP2833x_Sci.pp" ".\DSP2833x_SysCtrl.pp" ".\DSP2833x_Xintf.pp" ".\MotorCanCtrl.pp" ".\algrithom.pp" ".\maincontrol_dsp.pp" ".\uart_fpga.pp" 
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
