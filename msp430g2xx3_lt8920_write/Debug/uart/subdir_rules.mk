################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
uart/uscia0_uart.obj: ../uart/uscia0_uart.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/ti/ccsv6/tools/compiler/msp430_15.12.3.LTS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="D:/ti/ccsv6/ccs_base/msp430/include" --include_path="D:/ti/ccsv6/tools/compiler/msp430_15.12.3.LTS/include" --advice:power="all" -g --define=__MSP430G2955__ --diag_wrap=off --display_error_number --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="uart/uscia0_uart.d" --obj_directory="uart" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


