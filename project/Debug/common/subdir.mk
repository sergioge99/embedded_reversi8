################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../common/44blib.c \
../common/AscII6x8.c \
../common/AscII8x16.c 

ASM_SRCS += \
../common/44binit.asm 

OBJS += \
./common/44binit.o \
./common/44blib.o \
./common/AscII6x8.o \
./common/AscII8x16.o 

C_DEPS += \
./common/44blib.d \
./common/AscII6x8.d \
./common/AscII8x16.d 

ASM_DEPS += \
./common/44binit.d 


# Each subdirectory must supply rules for building sources it contributes
common/%.o: ../common/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC Assembler'
	arm-none-eabi-gcc -x assembler-with-cpp -I"C:\hlocal\workspace\entrega\common" -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

common/%.o: ../common/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -I"C:\hlocal\workspace\entrega\common" -O3 -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -mapcs-frame -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


