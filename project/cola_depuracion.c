/*********************************************************************************************
* Fichero:	cola_depuracion.c
* Autor1:	Sergio García Esteban	755844
* Autor2:	Irene Fumanal Lacoma	758325
* Descrip:	Cola circular que guarda eventos, informacion e instante de tiempo.
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "cola_depuracion.h"
#include "timer2.h"
#include "44b.h"
#include "44blib.h"
#include "def.h"
#include "option.h"


/* Ini cola circular 	_ISR_STARTADDRESS-0xf00-256*9
 * 256x8
 * Fin cola 			_ISR_STARTADDRESS-0xf00-256
 * 256
 * UserStack 			_ISR_STARTADDRESS-0xf00
 */

/*--- valores y constantes ---*/
enum{	MAXEVENTOS = 256 };	//N maximo de elementos
enum{	finPila = _ISR_STARTADDRESS-(0xf00)-(256) };
enum{	direccion = finPila-(MAXEVENTOS*8) };

/* Registro creado para almacenar los 2 enteros que guardamos de cada evento*/
typedef struct elemento_pila{
	uint32_t ID_evento_auxData;		//32 bits: 8 ID_evento + 24 auxData
	unsigned int momento_invocacion;//instante(timer2) de guardado del evento en pila
} elemento_pila;

/*--- variables globales del modulo ---*/
static elemento_pila *lista_eventos;
static int head;			//indice primer evento lista
static int tail;			//indice ultimo evento lista
static int n;				//numero de elementos apilados
// N SIMPLIFICA LA IMPLEMENTACION


/*--- codigo de las funciones ---*/

/*********************************************************************************************
* Inicializacion de la cola. La deja lista para ser usada.
*********************************************************************************************/
void cola_ini(){
	lista_eventos=direccion;	//asignamos direccion inicial de la cola
	head=0;
	tail=0;
	n=0;
}

/*********************************************************************************************
* Guarda un evento en la cola.
*********************************************************************************************/
void push_debug(ID_evento evento, uint32_t auxData){
	unsigned int time = timer2_leer();		 //instante(timer2)

	uint32_t ID_data = auxData&(0x00ffffff); //escribimos los 24 ultimos bits
	ID_data |= (evento<<24);				 //escribimos los 8 primeros bits

	elemento_pila nuevo;
	nuevo.ID_evento_auxData=ID_data;
	nuevo.momento_invocacion=time;
	lista_eventos[tail]=nuevo;				 //guardamos en cola

	if(tail==MAXEVENTOS){//si hemos escrito en la ultima pos, la siguiente escribirá en la pos 0
		tail=0;
	}else{
		tail++;
	}
	n++;
}

/*********************************************************************************************
* Lee un evento de la cola.
*********************************************************************************************/
void pop_debug(ID_evento *evento, uint32_t *auxData, unsigned int *time){
	if(n!=0){
		uint32_t get_ID_Data=lista_eventos[head].ID_evento_auxData;//leemos evento
		*evento=get_ID_Data>>24;			//nos quedamos con los 8 primeros bits
		*auxData=get_ID_Data&(0x00ffffff);	//nos quedamos con los 24 ultimos bits

		*time=lista_eventos[head].momento_invocacion;//leemos instante

		if(head==MAXEVENTOS){//si hemos leido en la ultima pos, la siguiente leerá en la pos 0
			head=0;
		}else{
			head++;
		}
		n--;
	}
}

/*********************************************************************************************
* Devuelve true si hay eventos en la cola que no han sido leidos.
*********************************************************************************************/
int hayEvento(){
	return n!=0;
}
