/*********************************************************************************************
* Fichero:	button.c
* Autor1:	Sergio García Esteban	755844
* Autor2:	Irene Fumanal Lacoma	758325
* Descrip:	Funciones de manejo de los pulsadores (EINT6-7)
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "button.h"
#include "44blib.h"
#include "44b.h"
#include "def.h"
#include "cola_depuracion.h"
#include "medicion_retardos.h"


/* declaración de función que es rutina de servicio de interrupción*/
void Eint4567_ISR() __attribute__((interrupt("IRQ")));


/*--- codigo de funciones ---*/

/*********************************************************************************************
* ISR EINT 6-7. Guarda un evento ev_pulsacion en la cola de depuracion.
* Desactiva las interrupciones de los botones.
*********************************************************************************************/
void Eint4567_ISR()
{
	#ifdef MEDICION_RETARDOS
	#else
		//desactivar interrupciones
		rINTMSK |= (BIT_EINT4567);
	#endif

	/* Identificar la interrupcion (hay dos pulsadores)*/
	int which_int = rEXTINTPND;	// VALORES: 4-> IZQUIERDO, 8-> DERECHO.
	if(rEXTINTPND==4 || rEXTINTPND==8){
		push_debug(ev_pulsacion,which_int/4); // enviamos el evento al gestor de eventos
	}else{
		rINTMSK &= ~(BIT_EINT4567);
	}


	/* Finalizar ISR */
	rEXTINTPND = 0xf;				// borra los bits en EXTINTPND
	rI_ISPC   |= BIT_EINT4567;		// borra el bit pendiente en INTPND
}

/*********************************************************************************************
* Configura los registros de los botones y los deja listos para usar.
*********************************************************************************************/
void button_iniciar()
{
	/* Configuracion del controlador de interrupciones. Estos registros están definidos en 44b.h */
	rEXTINTPND = 0xf;				// borra los bits en EXTINTPND
	rI_ISPC   |= BIT_EINT4567;		// borra el bit pendiente en INTPND
	rINTMSK    &= ~(BIT_EINT4567); 	// habilitamos interrupcion linea eint4567 en vector de mascaras (valor 0)

	/* Establece la rutina de servicio para Eint4567 */
	pISR_EINT4567 = (int) Eint4567_ISR;

	/* Configuracion del puerto G */
	rPCONG  |= 0xff00;		// Establece la funcion de los pines (EINT4-7)
	rPUPG   |= 0xf0;		// Deshabilita el "pull up" de los pines (EINT4-7)
	rEXTINT |= 0x22220000;	// Lineas de int. como de flanco de bajada (EINT4-7)

	/* Por precaucion, se vuelven a borrar los bits de INTPND y EXTINTPND */
	rEXTINTPND = 0xf;				// borra los bits en EXTINTPND
	rI_ISPC   |= BIT_EINT4567;		// borra el bit pendiente en INTPND
}

/*********************************************************************************************
* Reactiva las interrupciones de lls botones.
*********************************************************************************************/
void button_resetear(){
	rEXTINTPND = 0xf;				// borra los bits en EXTINTPND
	rI_ISPC   |= BIT_EINT4567;		// borra el bit pendiente en INTPND

	rINTMSK &= ~(BIT_EINT4567);		// activar interrupciones
}

/*********************************************************************************************
* Devuelve el estado actual de los botones.
*********************************************************************************************/
enum estado_button button_estado(){
	int data=rPDATG;
	if( (data & 0x80) == 0 ){		// comprobamos BIT 7, si está a 0 está pulsado el derecho
		return button_dr;
	}else if( (data & 0x40) == 0 ){	// comprobamos BIT 6, si está a 0 está pulsado el izquierdo
		return button_iz;
	}else{
		return button_none;
	}
}

