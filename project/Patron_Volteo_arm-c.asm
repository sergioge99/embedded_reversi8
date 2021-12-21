/*********************************************************************************************
* Fichero:	Patron_volteo_arm-arm.asm
* Autor1:	Sergio García Esteban	755844
* Autor2:	Irene Fumanal Lacoma	758325
* Descrip:	Funcion Patron_volteo optimizada en ensamblador manteniendo llamada a ficha_valida.
* Version:	Cheto
*********************************************************************************************/

.global patron_volteo_arm_c

patron_volteo_arm_c:

		mov r12, sp //prologo
		push {r4-r10,r11, r12, lr, pc}
		sub r11, r12, #4
		sub sp, sp, #24
		str r0, [r11, #-52]
		str r1, [r11, #-56]
		strb r2, [r11, #-57]
		strb r3, [r11, #-58]
		
		mov r6,r1
		ldrb r4, [r11, #4] //FA=FA+SF
		add r1,r2,r4
		bic r1,r1,#256//convertir a char
		ldrb r5, [r11, #8] //CA=CA+SC
		add r2,r3,r5
		bic r2,r2,#256
		
		mov r8,r1 //casilla = ficha_valida(tablero, FA, CA, &posicion_valida);
		mov r9,r2
		sub r3,r11,#48//fp-48=&posicion_valida
		mov r7,r0
		bl ficha_valida//llamada a c
		
		ldr r10, [r11, #12]//r10=color (parametro de entrada patron_volteo por pila)
		b cond //while ((posicion_valida == 1) && (casilla != color))
		
while_:	add r1,r8,r4 //FA=FA+SF
		bic r1,r1,#256
		add r2,r9,r5 //CA=CA+SC
		bic r2,r2,#256
		ldr r0,[r6] //*longitud = *longitud + 1;
		add r0,r0,#1
		str r0,[r6]
		
		mov r8,r1 //casilla = ficha_valida(tablero, FA, CA, &posicion_valida);
		mov r9,r2
		sub r3,r11,#48//fp-48=&posicion_valida
		mov r0,r7
		bl ficha_valida //llamada a c
		
		//while ((posicion_valida == 1) && (casilla != color))
cond:	ldr r3, [r11, #-48] //posicion_valida
		cmp r3,#1
		bne if_
		cmp r0,r10//r0=casilla(salida de ficha_valida())
		bne while_
		
if_:	cmp r3,#1 //if ((posicion_valida == 1) && (casilla == color) && (*longitud >0))
		bne else_
		cmp r0,r10
		bne else_
		ldr r6,[r6]
		cmp r6,#0
		ble else_
		
		mov r0,#1 //return PATRON_ENCONTRADO;
		b fin
		
else_:	mov r0,#0 //return NO_HAY_PATRON;
		
fin:	sub sp, r11, #40//epilogo
		ldm sp, {r4-r10,r11, sp, lr}
		bx lr
		
		
		
