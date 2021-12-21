/*********************************************************************************************
* Fichero:		reversi_main.c
* Autor1:		Sergio García Esteban	755844
* Autor2:		Irene Fumanal Lacoma	758325
* Descrip:		Gestor de eventos.
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "44blib.h"
#include "44b.h"
#include "reversi_main.h"
#include "jugada_por_pantalla.h"
#include "cola_depuracion.h"
#include "latido.h"
#include "botones_antirebotes.h"
#include "tp_antirebotes.h"
#include "pantalla.h"
#include <stdint.h>


/*
 * Configura el procesador en modo ahorro de energia
 */
void dormir_procesador(){
	//modo de bajo consumo de energía
	rCLKCON |= 0x404; // GPIO + IDLE
}

/*
 * Comprueba si hay eventos y los procesa segun el tipo de evento.
 */
void reversi_main(){

	while(1){
		while(hayEvento()){
			ID_evento evento;
			uint32_t info;
			unsigned int t;
			pop_debug(&evento,&info,&t);//obtenemos evento, info e instante

			if (evento==ev_tick0){//-------------------------------------------------------
				LCD_Refresca_ttotal();			// comprueba si hay que refrescar el ttotal
				LCD_imprimir();					// IMPRIME pantalla si hay cambios
				Latido_ev_new_tick();			// latido led
				antirrebotes_ev_tick();			// antirrebotes botones
				tp_antirrebotes_ev_tick();		// antirrebotes tactil
			}else if (evento==ev_pulsacion){//---------------------------------------------
				antirrebotes_ev_boton();		// antirrebotes botones
				jugada_x_pantalla(evento,info);	// AUTÓMATA juego
			}else if (evento==ev_mantener){//----------------------------------------------
				jugada_x_pantalla(evento,info);	// AUTÓMATA juego
			}else if (evento==ev_tactil){//------------------------------------------------
				tp_antirrebotes_ev_tactil();	// antirrebotes tactil
				jugada_x_pantalla(evento,info);	// AUTÓMATA juego
			}else if (evento==ev_dma){//---------------------------------------------------
				LCD_DMA_ready();				// comunica que dma ha terminado
			}
		}
		dormir_procesador();
	}
}



