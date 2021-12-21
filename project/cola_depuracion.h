/*********************************************************************************************
* Fichero:	cola_depuracion.h
* Autor1:	Sergio García Esteban	755844
* Autor2:	Irene Fumanal Lacoma	758325
* Descrip:	Cabecera cola circular que guarda eventos, informacion e instante de tiempo.
* Version:
*********************************************************************************************/

#ifndef _COLA_DEPURACION_H_
#define _COLA_DEPURACION_H_

#include <stdint.h>

typedef enum {ev_none=0, ev_tick0=1, ev_pulsacion=2, ev_mantener=3,
	ev_tactil=4, ev_dma=5} ID_evento;

/*--- declaracion de funciones visibles del módulo ---*/
void cola_ini();
void push_debug(ID_evento evento, uint32_t auxData);
void pop_debug(ID_evento *evento, uint32_t *auxData, unsigned int *time);
int hayEvento();

#endif /*_COLA_DEPURACION_ */
