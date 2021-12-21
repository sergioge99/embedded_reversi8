/*********************************************************************************************
* Fichero:	timer2.c
* Autor1:	Sergio García Esteban	755844
* Autor2:	Irene Fumanal Lacoma	758325
* Descrip:	Funciones de control del timer2
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "timer2.h"
#include "44b.h"
#include "44blib.h"

/*--- valores y constantes ---*/
enum { CUENTA_INICIAL = 65535 };
enum { CPU_FREQ = 64 };
enum { DIVIDER = 2 };
enum { TICK_TO_MS = CPU_FREQ/DIVIDER };

/*--- variables globales ---*/
volatile int timer2_num_int;// volatile para que sea modificable concurrentemente

/*--- ISR ---*/
void timer2_ISR() __attribute__((interrupt("FIQ")));

/*--- codigo de las funciones ---*/

/*
 * ISR TIMER2. Actualiza el contador.
 */
void timer2_ISR()
{
	timer2_num_int++;

	/* borrar bit en I_ISPC para desactivar la solicitud de interrupción*/
	rF_ISPC |= BIT_TIMER2; // BIT_TIMER2 pone un 1 en el bit 13 que correponde al Timer2
}

/*
 * Configura los registros del timer0 y lo deja listo para ser usado.
 */
void timer2_inicializar()
{
	/* Configuracion controlador de interrupciones */
	rINTMSK &= ~(BIT_TIMER2); // habilitamos en vector de mascaras de interrupcion el Timer0 (bits 26 y 13, BIT_GLOBAL y BIT_TIMER2 están definidos en 44b.h)

	/* Establece la ISR del TIMER2 */
	pISR_FIQ = (unsigned) timer2_ISR;

	/* Configura el Timer2 */
	rTCFG0 &=  ~(0xFF<<8); 	// preescalado timer2 (bits 8-15) valor 0
	rTCFG1 &=  ~(0xF<<8); 	// entrada del mux2 (bits 8-11) valor 0 (divisor 2)
	rTCNTB2 = CUENTA_INICIAL;//valor inicial de cuenta
	rTCMPB2 = 0;			// valor de comparación

	rTCON &=  ~(0xF<<12);	// 4 bits timer2 (bits 12-15) a 0
	rTCON |=  (0x2<<12);	// manual-update 1
	rTCON &=  ~(0x2<<12);	// manual-update 0
	rTCON |=  (0x8<<12);	// auto-reload 1
}

/*
 * Inicia la ejecucion del timer2.
 */
void timer2_empezar(void)
{
	timer2_num_int=0;

	rTCON &=  ~(0xF<<12);	// 4 bits timer2 (bits 12-15) a 0
	rTCON |=  (0x2<<12);	// manual-update 1
	rTCON &=  ~(0x2<<12);	// manual-update 0
	rTCON |=  (0x8<<12);	// auto-reload 1
	rTCON |=  (0x1<<12);	// start 1
}

/*
 * Devuelve el tiempo en us que lleva ejecutando el timer2.
 * Garantiza el resultado correcto, incluso si llega una interrupcion del timer2 durante
 * la lectura, solo si se ejecutan las instrucciones en el orden escrito.
 */
unsigned int timer2_leer()
{
	unsigned int primera_lectura = timer2_num_int;
	unsigned int cuenta = rTCNTO2;
	unsigned int segunda_lectura = timer2_num_int;
	if(primera_lectura==segunda_lectura){
		return ( CUENTA_INICIAL*primera_lectura + (CUENTA_INICIAL-cuenta) )/(TICK_TO_MS);
	}else{
		return ( CUENTA_INICIAL*segunda_lectura + (CUENTA_INICIAL-rTCNTO2) )/(TICK_TO_MS);
	}
}

/*
 * Detiene la ejecucion del timer2. Devuelve el tiempo en us que lleva ejecutando.
 */
unsigned int timer2_parar()
{
	rTCON = rTCON & ~(0x1<<12);	// start 0 (bits 12-15 de timer2)
	return timer2_leer();
}
