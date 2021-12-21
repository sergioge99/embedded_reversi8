################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../8led.c \
../Bmp.c \
../botones_antirebotes.c \
../button.c \
../cola_depuracion.c \
../exception_lib.c \
../itoa.c \
../jugada_por_pantalla.c \
../latido.c \
../lcd.c \
../led.c \
../main.c \
../pantalla.c \
../prueba_timer2.c \
../pruebas_patron_volteo.c \
../reversi8_2019.c \
../reversi_main.c \
../timer.c \
../timer2.c \
../tp.c \
../tp_antirebotes.c 

ASM_SRCS += \
../Patron_Volteo_arm-c.asm \
../Patron_volteo_arm-arm.asm 

OBJS += \
./8led.o \
./Bmp.o \
./Patron_Volteo_arm-c.o \
./Patron_volteo_arm-arm.o \
./botones_antirebotes.o \
./button.o \
./cola_depuracion.o \
./exception_lib.o \
./itoa.o \
./jugada_por_pantalla.o \
./latido.o \
./lcd.o \
./led.o \
./main.o \
./pantalla.o \
./prueba_timer2.o \
./pruebas_patron_volteo.o \
./reversi8_2019.o \
./reversi_main.o \
./timer.o \
./timer2.o \
./tp.o \
./tp_antirebotes.o 

C_DEPS += \
./8led.d \
./Bmp.d \
./botones_antirebotes.d \
./button.d \
./cola_depuracion.d \
./exception_lib.d \
./itoa.d \
./jugada_por_pantalla.d \
./latido.d \
./lcd.d \
./led.d \
./main.d \
./pantalla.d \
./prueba_timer2.d \
./pruebas_patron_volteo.d \
./reversi8_2019.d \
./reversi_main.d \
./timer.d \
./timer2.d \
./tp.d \
./tp_antirebotes.d 

ASM_DEPS += \
./Patron_Volteo_arm-c.d \
./Patron_volteo_arm-arm.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -I"C:\hlocal\workspace\entrega\common" -O3 -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -mapcs-frame -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC Assembler'
	arm-none-eabi-gcc -x assembler-with-cpp -I"C:\hlocal\workspace\entrega\common" -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


