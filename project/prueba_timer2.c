/*********************************************************************************************
* Fichero:		prueba_timer2.c
* Autor1:	Sergio García Esteban	755844
* Autor2:	Irene Fumanal Lacoma	758325
* Descrip:		Funciones de debugg de timer2.
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "timer2.h"
#include "44blib.h"

/*--- valores y constantes ---*/
enum{ TIEMPO1=1000 };
enum{ TIEMPO2=10000 };
enum{ TIEMPO3=1000000 };
enum{ TIEMPO4=10000000 };
enum{ DELAY_UNIT=100 };//Delay 100us resolution

/*
 * Prueba el funcionamiento del timer2 comparando los tiempos con los de Delay()
 * Si no coinciden entra en bucle infinito.
 */
void probar_timer2(){
	unsigned int t1;
	unsigned int t2;
	unsigned int t3;
	unsigned int t4;
	timer2_inicializar();

	timer2_empezar();
	Delay(TIEMPO1/DELAY_UNIT);
	t1 = timer2_leer();//1000

	timer2_empezar();
	Delay(TIEMPO2/DELAY_UNIT);
	t2 = timer2_leer();//10000

	timer2_empezar();
	Delay(TIEMPO3/DELAY_UNIT);
	t3 = timer2_leer();//1000000

	timer2_empezar();
	Delay(TIEMPO4/DELAY_UNIT);
	t4 = timer2_parar();//10000000

	// Si hay un error mayor al 5%, entra en bucle infinito.
	if( t1<(TIEMPO1*0.95) || t1>(TIEMPO1*1.05) )	while(1);
	if( t2<(TIEMPO2*0.95) || t2>(TIEMPO2*1.05) )	while(1);
	if( t3<(TIEMPO3*0.95) || t3>(TIEMPO3*1.05) )	while(1);
	if( t4<(TIEMPO4*0.95) || t4>(TIEMPO4*1.05) )	while(1);

}

