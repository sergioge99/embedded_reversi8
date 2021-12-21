/*********************************************************************************************
* Fichero:		main.c
* Autor1:	Sergio García Esteban	755844
* Autor2:	Irene Fumanal Lacoma	758325
* Descrip:		MAIN Proyecto reversi
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "8led.h"
#include "led.h"
#include "button.h"
#include "cola_depuracion.h"
#include "timer2.h"
#include "timer.h"
#include "exception_lib.h"
#include "44blib.h"
#include "44b.h"
#include "lcd.h"
#include "tp.h"

#include "reversi_main.h"
#include "pruebas_patron_volteo.h"
#include "prueba_timer2.h"


/*--- codigo de funciones ---*/

/*
 * Cambia a modo usuario
 */
void modo_usuario(){
	__asm__ volatile("mrs r0, cpsr");		// Copiamos el cpsr al spsr
	__asm__ volatile("msr spsr, r0");		// Reasignamos módulo C del cpsr, asignamos
	__asm__ volatile("msr cpsr_c, 0x10");	// a bits de modo el modo usuario.
}

/*
 * Inicializa sistema y perifericos, y lanza el juego.
 */
void Main(void)
{
	/* --- Inicialización del sistema --- */
	sys_init();	// (placa, interrupciones y puertos)

	/* --- Inicialización de la cola de eventos --- */
	cola_ini();

	/* --- Configuracion de ISR de Excepciones --- */
	exception_init();

	/* --- Inicialización de los dispositivos --- */
	button_iniciar();
	D8Led_init();
	timer2_inicializar();
	Lcd_Init();
	touchpad_init();

	/* --- Iniciamos los timers --- */
	timer_init();
	timer2_empezar();


	//////////////////////////////////////////////////////////////////
	//	Los siguientes comentarios contienen codigo descomentable.	//
	//////////////////////////////////////////////////////////////////
	/* --- Debug timer2 --- */
	//probar_timer2();					//si falla, bucle infinito

	/* --- Debug volteo --- */
	//pruebas_patron_volteo();			//si falla, bucle infinito
	//pruebas_tiempo_patron_volteo();	//poner brkpoint para ver tiempos


	/* --- Generador excepciones --- */
	//asm volatile("SWI #0");				//SWI 1

	//unsigned int x=1;
	//unsigned int y=0;						//Data Abort 2
	//asm volatile("ldr r0, [%1,%2]" :"=r"(x)  :"r"(x) ,"r"(y));

	//asm volatile(".word 0xe7f000f0\n"); 	//UND 3

	//////////////////////////////////////////////////////////////////////////




	/* --- Modo usuario --- */
	modo_usuario();
	/* --- Juego --- */
	reversi_main();




}
