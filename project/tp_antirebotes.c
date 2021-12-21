/*********************************************************************************************
* Fichero:	tp_antirebotes.c
* Autor1:	Sergio García Esteban	755844
* Autor2:	Irene Fumanal Lacoma	758325
* Descrip:	Autómata filtro de interrupciones del tp
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "tp_antirebotes.h"
#include "tp.h"
#include "cola_depuracion.h"

/*--- valores y constantes ---*/
enum estado{ TP_PREPARADO, RETARDO_PULSAR, ESPERANDO_SOLTAR, RETARDO_SOLTAR };
enum{ MS_30 = 2 };			//VALORES PARA TRANSFORMAR DE TIMER0_TICK A MS
enum{ TRP = 8, TRD = 4 };	//VALORES DE LOS RETARDOS EN TIMER0_TICK

/*--- variables globales del módulo ---*/
static int estado=TP_PREPARADO;
static int counter=0;

/*--- codigo de funciones ---*/

/*********************************************************************************************
* Funcion 1 del autómata. Transicion causada por ev_tactil.
*********************************************************************************************/
void tp_antirrebotes_ev_tactil()
{
	estado=RETARDO_PULSAR; //Han pulsado boton, pasamos a estado de espera de retardo inicial
	counter=0;
}

/*********************************************************************************************
* Funcion 2 del autómata. Transicion causada por ev_tick. Gestiona la reactivación
* de las interrupciones del touchpad.
*********************************************************************************************/
void tp_antirrebotes_ev_tick()
{
	if(estado!=TP_PREPARADO){
		counter++;
		if(estado==RETARDO_PULSAR && counter==TRP){		// espera retardo inical
			estado=ESPERANDO_SOLTAR;
		}else if(estado==ESPERANDO_SOLTAR && counter%MS_30){
			if(touchpad_estado()==0){					// si no esta pulsado, cambio de estado
				estado=RETARDO_SOLTAR;
				counter=0;
			}
		}else if(estado==RETARDO_SOLTAR && counter==TRD){//espera retardo final
			estado=TP_PREPARADO;
			touchpad_reactivar();						// activamos interrupciones
		}
	}
}
