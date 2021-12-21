/*********************************************************************************************
* Fichero:	botones_antirebotes.c
* Autor1:	Sergio García Esteban	755844
* Autor2:	Irene Fumanal Lacoma	758325
* Descrip:	Autómata filtro de interrupciones de los botones
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "botones_antirebotes.h"
#include "button.h"
#include "cola_depuracion.h"

/*--- valores y constantes ---*/
enum estado{ BOTON_PREPARADO, RETARDO_PULSAR, ESPERANDO_SOLTAR, RETARDO_SOLTAR };
enum{ MS_30 = 2, MS_333 = 21, MS_180 = 12 };	//VALORES PARA TRANSFORMAR DE TIMER0_TICK A MS
enum{ TRP = 12, TRD = 2 };	//VALORES DE LOS RETARDOS EN TIMER0_TICKS

/*--- variables globales del módulo ---*/
static int estado;
static int counter;

/*--- codigo de funciones ---*/

/*********************************************************************************************
* Funcion 1 del autómata. Transicion causada por ev_boton.
*********************************************************************************************/
void antirrebotes_ev_boton()
{
	estado=RETARDO_PULSAR; //Han pulsado boton, pasamos a estado de espera de retardo inicial
	counter=0;
}

/*********************************************************************************************
* Funcion 2 del autómata. Transicion causada por ev_tick. Gestiona la reactivación
* de las interrupciones de los botones.
*********************************************************************************************/
void antirrebotes_ev_tick()
{
	if(estado!=BOTON_PREPARADO){
		counter++;
		if(estado==RETARDO_PULSAR && counter==TRP){//espera retardo inical
			estado=ESPERANDO_SOLTAR;
		}else if(estado==ESPERANDO_SOLTAR && counter%MS_30){
			enum estado_button b_actual = button_estado();//comprueba estado actual de los botones
			if(b_actual==button_none){//si no esta pulsado ejecutamos transicion de estado
				estado=RETARDO_SOLTAR;
				counter=0;
			}
			else{	//OPCIONAL 1:   si mantiene pulsado se lo comunicamos al gestor de eventos
			  	if( (counter>=(MS_333) )&&( (counter-MS_333)%MS_180==0 )){
			  		push_debug(ev_mantener,b_actual);
			 	}
			}
		}else if(estado==RETARDO_SOLTAR && counter==TRD){//espera retardo final
			estado=BOTON_PREPARADO;
			button_resetear();//activamos interrupciones
		}
	}
}
