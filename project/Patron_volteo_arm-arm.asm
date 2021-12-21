/*********************************************************************************************
* Fichero:	Patron_volteo_arm-arm.asm
* Autor1:	Sergio García Esteban	755844
* Autor2:	Irene Fumanal Lacoma	758325
* Descrip:	Funcion Patron_volteo optimizada en ensamblador.
* Version:	Cheto
*********************************************************************************************/

.global patron_volteo_arm_arm

/****************************************
* r0 = parametro 0 (r0) = @tablero	
* r1 = parametro 1 (r1) = @longitud
* r2 = parametro 2 (r2) = FA
* r3 = parametro 3 (r3) = CA
* r4 = posicion_valida
* r5 = parametro 4 (fp+4) = SF 
* r6 = parametro 5 (fp+8) = SC
* r7 = casilla 
* r8 = parametro 6 (fp+12) = color
* r9 = tablero[FA][CA]
* r10 = FA*8 || longitud
****************************************/

patron_volteo_arm_arm:

		mov r12, sp //prologo
		push {r4-r10,r11, r12, lr, pc}
		sub r11, r12, #4
		
		ldrb r5, [r11, #4] //FA=FA+SF
		add r2,r2,r5
		bic r2,r2,#256
		ldrb r6, [r11, #8] //CA=CA+SC
		add r3,r3,r6
		bic r3,r3,#256
		ldr r8, [r11, #12]

		//ficha_valida
		mov r4,#0 //pos_v=0
		cmp r2,#7 //0<=FA<DIM
		bhi fv1_f
		cmp r3,#7 //0<=CA<DIM
		bhi fv1_f
		lsl r10,r2,#3
		add r9,r0,r10
		ldrb r7,[r9,r3]
		cmp r7,#0 //tablero[FA][CA]!=0=casilla
		movne r4,#1 //pos_v=1
		
fv1_f:	b cond //while ((posicion_valida == 1) && (casilla != color))
		
while_:	add r2,r2,r5 //FA=FA+SF
		bic r2,r2,#256
		add r3,r3,r6 //CA=CA+SC
		bic r3,r3,#256
		ldr r10,[r1] //*longitud = *longitud + 1;
		add r10,r10,#1
		str r10,[r1]
		
		//ficha_valida
		mov r4,#0 //pos_v=0
		cmp r2,#7 //0<=FA<DIM
		bhi cond
		cmp r3,#7 //0<=CA<DIM
		bhi cond
		lsl r10,r2,#3
		add r9,r0,r10
		ldrb r7,[r9,r3]
		cmp r7,#0 //tablero[FA][CA]!=0=casilla
		movne r4,#1 //pos_v=1
		
cond:	cmp r4,#1 //while ((posicion_valida == 1) && (casilla != color))
		bne if_
		cmp r7,r8
		bne while_
		
if_:	mov r0,#0 //return NO_HAY_PATRON;
		cmp r4,#1 //if ((posicion_valida == 1) && (casilla == color) && (*longitud >0))
		bne fin
		cmp r7,r8
		bne fin
		ldr r1,[r1]
		cmp r1,#0
		movgt r0,#1 //return PATRON_ENCONTRADO;
		
fin:	sub sp, r11, #40 //epilogo
		ldm sp, {r4-r10,r11, sp, lr}
		bx lr
		
		
		
