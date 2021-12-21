/*********************************************************************************************
* Fichero:		timer2.h
* Autor:
* Descrip:		funciones de control del timer2 del s3c44b0x
* Version:
*********************************************************************************************/

#ifndef _TIMER2_H_
#define _TIMER2_H_

/*--- declaracion de funciones visibles del módulo ---*/
void timer2_inicializar();
void timer2_empezar();
unsigned int timer2_leer();
unsigned int timer2_parar();

#endif /* _TIMER2_H_ */
