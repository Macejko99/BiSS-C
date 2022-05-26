################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs1100/ccs/tools/compiler/ti-cgt-c2000_21.6.0.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./main.obj" \
"./mcu.obj" \
"./spi_communication.obj" \
"./TI_Files/F2837xD_CodeStartBranch.obj" \
"./TI_Files/F2837xD_GlobalVariableDefs.obj" \
"./TI_Files/F2837xD_usDelay.obj" \
"../TI_Files/2837xD_RAM_lnk_cpu1.cmd" \
"../TI_Files/F2837xD_Headers_nonBIOS_cpu1.cmd" \
$(GEN_CMDS__FLAG) \
-lrts2800_fpu32_eabi.lib \
-l"C:/ti/controlSUITE/libs/app_libs/position_manager/v01_00_00_00/bissc/Float/lib/PM_bissC_Lib.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include TI_Files/subdir_vars.mk
-include subdir_rules.mk
-include TI_Files/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
Komunikácia\ snímača\ pomocou\ BiSS-C.out \

EXE_OUTPUTS__QUOTED += \
"Komunikácia snímača pomocou BiSS-C.out" \

BIN_OUTPUTS += \
Komunikácia\ snímača\ pomocou\ BiSS-C.hex \

BIN_OUTPUTS__QUOTED += \
"Komunikácia snímača pomocou BiSS-C.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "Komunikácia snímača pomocou BiSS-C.out"

# Tool invocations
Komunikácia\ snímača\ pomocou\ BiSS-C.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ccs1100/ccs/tools/compiler/ti-cgt-c2000_21.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --advice:performance=all --define=CPU1 -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi -z -m"Komunikácia snímača pomocou BiSS-C.map" --stack_size=0x200 --warn_sections -i"C:/ti/ccs1100/ccs/tools/compiler/ti-cgt-c2000_21.6.0.LTS/lib" -i"C:/ti/ccs1100/ccs/tools/compiler/ti-cgt-c2000_21.6.0.LTS/include" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="Komunikácia snímača pomocou BiSS-C_linkInfo.xml" --rom_model -o "Komunikácia snímača pomocou BiSS-C.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

Komunikácia\ snímača\ pomocou\ BiSS-C.hex: $(EXE_OUTPUTS)
	@echo 'Building secondary target: "$@"'
	@echo 'Invoking: C2000 Hex Utility'
	"C:/ti/ccs1100/ccs/tools/compiler/ti-cgt-c2000_21.6.0.LTS/bin/hex2000" --diag_wrap=off -o "Komunikácia snímača pomocou BiSS-C.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building secondary target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "main.obj" "mcu.obj" "spi_communication.obj" "TI_Files\F2837xD_CodeStartBranch.obj" "TI_Files\F2837xD_GlobalVariableDefs.obj" "TI_Files\F2837xD_usDelay.obj" 
	-$(RM) "main.d" "mcu.d" "spi_communication.d" "TI_Files\F2837xD_GlobalVariableDefs.d" 
	-$(RM) "TI_Files\F2837xD_CodeStartBranch.d" "TI_Files\F2837xD_usDelay.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

