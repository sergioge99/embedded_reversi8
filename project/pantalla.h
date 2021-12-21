/*********************************************************************************************
* Fichero:		pantalla.h
* Autor1:		Sergio García Esteban	755844
* Autor2:		Irene Fumanal Lacoma	758325
* Descrip:		Cabecera funciones de pantalla.
* Version:
*********************************************************************************************/

#ifndef PANTALLA_H_
#define PANTALLA_H_

#include "reversi8_2019.h"

typedef enum { PANTALLA_INICIO, PANTALLA_JUEGO, PANTALLA_FINAL, PANTALLA_CALIBRADO} Pantalla;
/*--- declaracion de funciones visibles del módulo ---*/
void LCD_imprimir();
void LCD_pantalla(Pantalla p);
void LCD_movimiento(int f,int c);
void LCD_tablero(char t[DIM][DIM]);
void LCD_tcalculo(unsigned int t);
void LCD_tvolteo(unsigned int t);
void LCD_esquina(int e);
void LCD_fichas(int b, int n);
void LCD_DMA_ready();
void LCD_Hay_cambios();
void LCD_Refresca_ttotal();

#endif /* PANTALLA_H_ */
