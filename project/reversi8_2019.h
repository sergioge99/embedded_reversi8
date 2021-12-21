/*********************************************************************************************
* Fichero:	reversi8_2019.h
* Autor1:	Sergio García Esteban	755844
* Autor2:	Irene Fumanal Lacoma	758325
* Descrip:	Cabecera codigo del juego reversi
* Version:
*********************************************************************************************/

#ifndef _REVERSI8_2019_H_
#define _REVERSI8_2019_H_

/*--- definicion de valores ---*/
enum { DIM=8 };

enum {
	CASILLA_VACIA = 0,
	FICHA_BLANCA = 1,
	FICHA_NEGRA = 2
};

/*--- declaración de funciones visibles del módulo  ---*/
int reversi_procesar(char tablero[][DIM], char candidatas[][DIM], char fila, char columna);
void init_table(char tablero[][DIM], char candidatas[][DIM] );
void contar(char tablero[][DIM], int *b, int *n);
int movimiento_valido(char tablero[][DIM], char f, char c);

//Visible para realizar las pruebas en pruebas_patron_volteo
int patron_volteo(char tablero[][DIM], int *longitud, char FA, char CA, char SF, char SC, char color);

#endif /* _REVERSI8_2019_H_ */
