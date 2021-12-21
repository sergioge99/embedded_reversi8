/*********************************************************************************************
* Fichero:	button.h
* Autor1:	Sergio García Esteban	755844
* Autor2:	Irene Fumanal Lacoma	758325
* Descrip:	Cabecera funciones de manejo de los pulsadores (EINT6-7)
* Version:
*********************************************************************************************/

#ifndef _BUTTON_H_
#define _BUTTON_H_

/*--- variables visibles del módulo ---*/
static enum estado_button {button_none=0, button_iz=1, button_dr=2} estado_button;

/*--- declaracion de funciones visibles del módulo ---*/
void button_iniciar();
void button_resetear();
enum estado_button button_estado();

#endif /* _BUTTON_H_ */
