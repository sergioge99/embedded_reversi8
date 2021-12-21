/*********************************************************************************************
* Fichero:	exception_lib.c
* Autor1:	Sergio García Esteban	755844
* Autor2:	Irene Fumanal Lacoma	758325
* Descrip:	Tratamiento de excepciones.
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "exception_lib.h"
#include "44b.h"
#include "44blib.h"
#include "8led.h"

/*********************************************************************************************
* Declaramos la ISR para los 3 tipos de excepcion.
*********************************************************************************************/
void Handler1() __attribute__((interrupt("SWI")))
				__attribute__((interrupt("UNDEF")))
				__attribute__((interrupt("ABORT")));


/*--- codigo de las funciones ---*/

/*********************************************************************************************
* Inicializacion, asigna la rutina Handler1() a las 3 excepciones.
*********************************************************************************************/
void exception_init(){
	pISR_SWI = (unsigned) Handler1;
	pISR_DABORT = (unsigned) Handler1;
	pISR_UNDEF = (unsigned) Handler1;
}
/*********************************************************************************************
* Rutina de excepcion. Identifica el tipo de excepcion y la direccion donde es causada.
* Muestra por el 8led: 1 si es SWI, 2 si es Data Abort y 3 si es Undefined.
*********************************************************************************************/
void Handler1()
{
	int ExcepcionInterna=0;
	int instruccion=0;
	unsigned long cpsr;

	asm("mrs %0, cpsr":"=r"(cpsr):);	// leemos cpsr
	cpsr=cpsr%32;	// nos quedamos con el valor de los ultimos 5 bits del cpsr (modo)

	if(cpsr==19){		// SWI
		ExcepcionInterna=1;
		asm("movs %0, r14":"=r"(instruccion):);
	}
	else if(cpsr==23){	// Data Abort
		ExcepcionInterna=2;
		asm("movs %0, r14":"=r"(instruccion):);
		instruccion-=8;			//offset direccion en Data Abort
	}
	else if(cpsr==27){	// Undefined
		ExcepcionInterna=3;
		asm("movs %0, r14":"=r"(instruccion):);
	}


	while(1){	//8led parpadea numero de excepcion
		D8Led_symbol(ExcepcionInterna);
		Delay(500);
		D8Led_symbol(16);
		Delay(500);
	}
}
