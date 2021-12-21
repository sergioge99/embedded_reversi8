/*********************************************************************************************
* Fichero:	tp.h
* Autor1:	Sergio García Esteban	755844
* Autor2:	Irene Fumanal Lacoma	758325
* Descrip:	Cabecera funciones de manejo de la pantalla tactil
* Version:
*********************************************************************************************/
#include "def.h"
#include "44b.h"
#include "44blib.h"
#ifndef __TP_H__
#define __TP_H__

/*--- declaracion de funciones visibles del módulo ---*/
void touchpad_init();
void touchpad_setCalibrar(int Ncal);
int touchpad_estado();
void touchpad_reactivar();


#endif /*__TP_H__*/


