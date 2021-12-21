/*********************************************************************************************
* Fichero:	latido.c
* Autor1:	Sergio García Esteban	755844
* Autor2:	Irene Fumanal Lacoma	758325
* Descrip:	Latido de funcionamiento del juego
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "latido.h"
#include "led.h"

/*--- codigo de funciones ---*/

/*
 * Cuenta el tiempo y decide si enciende/apaga el led
 */
void Latido_ev_new_tick(){
	static int ev_new_tick=0;
	ev_new_tick++;
	if(ev_new_tick==8){			// primeros 8 ticks encendido
		led1_off();
	}else if(ev_new_tick>=15){	// ultimos 7 ticks apagado
		led1_on();
		ev_new_tick=0;
	}
}


