/*********************************************************************************************
* Fichero:	timer.c
* Autor1:	Sergio García Esteban	755844
* Autor2:	Irene Fumanal Lacoma	758325
* Descrip:	Funciones de control del timer0
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "timer.h"
#include "medicion_retardos.h"
#include "cola_depuracion.h"
#include "44b.h"
#include "44blib.h"

/*--- valores y constantes ---*/
enum{ CUENTA_INICIAL = 59259};

/*--- ISR ---*/
void timer_ISR(void) __attribute__((interrupt("IRQ")));

/*--- codigo de las funciones ---*/

/*
 * ISR TIMER0. Guarda un evento ev_tick0 en la cola de depuracion.
 */
void timer_ISR(void)
{
#ifdef MEDICION_RETARDOS
#else
	push_debug(ev_tick0,0);
#endif

	/* borrar bit en I_ISPC para desactivar la solicitud de interrupción*/
	rI_ISPC |= BIT_TIMER0; // BIT_TIMER0 está definido en 44b.h y pone un uno en el bit 13 que correponde al Timer0
}

/*
 * Configura los registros del timer0 e inicia la ejecucion.
 */
void timer_init()
{
	/* Configuracion controlador de interrupciones */
	rINTMSK &= ~(BIT_TIMER0); // habilitamos en vector de mascaras de interrupcion el Timer0 (bits 26 y 13, BIT_GLOBAL y BIT_TIMER0 están definidos en 44b.h)

	/* Establece la ISR del TIMER0 */
	pISR_TIMER0 = (unsigned) timer_ISR;

	/* Configura el Timer0 */
	rTCFG0 &= ~(0xFF);		 // CLEAN preescalado timer0 (bits 0-7)
	rTCFG0 |= (0x08);		 // preescalado 9 (8+1)
	rTCFG1 &= ~(0xF);		 // entrada del mux0(bits 0-3) 0000 DIVIDER 2
	rTCNTB0 = CUENTA_INICIAL;// valor inicial de cuenta
	rTCMPB0 = 0;			 // valor de comparación

	/* Iniciamos la ejecucion el Timer0 */
	rTCON &=  ~(0xF);		// CLEAN 4 bits correspondientes timer0 (bits 0-3)
	rTCON |=  (0x2);		// manual-update 1
	rTCON &=  ~(0x2);		// manual-update 0
	rTCON |=  (0x8);		// auto-reload 1
	rTCON |=  (0x1);		// start 1
}

