/*********************************************************************************************
* Fichero:	jugada_por_pantalla.c
* Autor1:	Sergio García Esteban	755844
* Autor2:	Irene Fumanal Lacoma	758325
* Descrip:	Autómata del juego
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "jugada_por_pantalla.h"
#include "reversi8_2019.h"
#include "8led.h"
#include "44blib.h"
#include "tp.h"
#include "pantalla.h"
#include "button.h"
#include "tp.h"


/*--- valores y constantes ---*/
enum estado{ INICIAL, JUEGO, FINAL, CALIBRADO };


/*--- codigo de funciones ---*/

/*********************************************************************************************
* Autómata de juego. Gestiona la interaccion del usuario (utilizando la pantalla y los botones)
* con la interfaz del juego y la partida.
*********************************************************************************************/
void jugada_x_pantalla(ID_evento evento, uint32_t info ){
	static int estado=CALIBRADO;
	static char fila=0,columna=0;
	static int mov_valido=0;
	static int x=0,y=0;
	static int fin=0;
	static int blancas=0,negras=0;
	static int esquina=0;
	if(evento==ev_tactil){
		x=info&0x00000fff;		// leemos coordenadas de la pulsacion en el tp
		y=((info&0x00fff000)>>12);
	}
	static char __attribute__ ((aligned (8))) candidatas[DIM][DIM] = {
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0}
	};
	static char __attribute__ ((aligned (8))) tablero[DIM][DIM] = {
			{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
			{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
			{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
			{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
			{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
			{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
			{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
			{CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}
	};
//-----------------------------------------------------------------------------------------------------
	if( (estado==INICIAL) & (evento==ev_tactil) & (220<x) & (x<=319) & (170<=y) & (y<=239) ){
		estado=CALIBRADO;					// INICIAL -> CALIBRADO
		touchpad_setCalibrar(4);			// tp_ISR calcula nuevos maximos en proximas 4 pulsaciones
		LCD_pantalla(PANTALLA_CALIBRADO);	// pantalla a imprimir
		esquina=0;
		LCD_esquina(esquina);				// esquina a imprimir
		LCD_Hay_cambios();					// CAMBIOS_LCD (indica a dma que hay que iniciar transmision)
//-----------------------------------------------------------------------------------------------------
	}else if( (estado==INICIAL) & (evento==ev_tactil)  ){
		estado=JUEGO;						// INICIAL -> JUEGO
		init_table(tablero,candidatas);		// inicializar partida
		fila=0;
		columna=0;
		LCD_pantalla(PANTALLA_JUEGO);		// pantalla a imprimir
		LCD_tablero(tablero);				// tablero a imprimir
		LCD_movimiento(fila,columna);		// ficha gris a imprimir
		LCD_Hay_cambios();					// CAMBIOS_LCD
//-----------------------------------------------------------------------------------------------------
	}else if( (estado==JUEGO) & ((evento==ev_pulsacion) | (evento==ev_mantener)) ){
		if(info==button_dr){				// JUEGO
			columna=(columna+1)%DIM;
		}else{
			fila=(fila+1)%DIM;
		}
		LCD_movimiento(fila,columna);		// ficha gris a imprimir
		LCD_Hay_cambios();					// CAMBIOS_LCD
//-----------------------------------------------------------------------------------------------------
	}else if( (estado==JUEGO) & (evento==ev_tactil) & (110<=x) ){
											// JUEGO
		mov_valido=movimiento_valido(tablero, fila, columna);
		fin=0;								// Comprobamos si el movimiento es válido
		if(mov_valido==1){					// Si es válido, ejecutamos movimiento y movimiento máquina.
			fin=reversi_procesar(tablero,candidatas,fila,columna);
		}else{								// Si no es valido, solo ejecuta movimiento la máquina.
			fin=reversi_procesar(tablero,candidatas,9,9);
		}
		if(fin==0){
			fila=0;
			columna=0;
			LCD_tablero(tablero);			// tablero a imprimir
			LCD_movimiento(fila,columna);	// ficha gris a imprimir
		}else{
			estado=FINAL;					// JUEGO -> FINAL
			contar(tablero,&blancas,&negras);
			LCD_fichas(blancas,negras);		// resultado a imprimir
			LCD_pantalla(PANTALLA_FINAL);	// pantalla a imprimir
		}
		LCD_Hay_cambios();					// CAMBIOS_LCD
//-----------------------------------------------------------------------------------------------------
	}else if( (estado==JUEGO) & (evento==ev_tactil) & (x<=109) ){
		estado=INICIAL;						// JUEGO -> INICIAL
		LCD_pantalla(PANTALLA_INICIO);		// pantalla a imprimir
		LCD_Hay_cambios();					// CAMBIOS_LCD
//-----------------------------------------------------------------------------------------------------
	}else if( (estado==CALIBRADO) & (evento==ev_tactil) ){
		esquina++;							// CALIBRADO
		LCD_esquina(esquina);				// esquina a imprimir
		if(esquina==4){
			estado=INICIAL;					// CALIBRADO -> INICIAL
			LCD_pantalla(PANTALLA_INICIO);	// pantalla a imprimir
		}
		LCD_Hay_cambios();					// CAMBIOS_LCD
//-----------------------------------------------------------------------------------------------------
	}else if( (estado==FINAL) & (evento==ev_tactil) & (120<=y) ){
		estado=JUEGO;						// FINAL -> JUEGO
		init_table(tablero,candidatas);		// inicializar partida
		fila=0;
		columna=0;
		LCD_pantalla(PANTALLA_JUEGO);		// pantalla a imprimir
		LCD_tablero(tablero);				// tablero a imprimir
		LCD_movimiento(fila,columna);		// ficha gris a imprimir
		LCD_Hay_cambios();					// CAMBIOS_LCD
//-----------------------------------------------------------------------------------------------------
	}else if( (estado==FINAL) & (evento==ev_tactil) & (y<=119) ){
		estado=INICIAL;						// FINAL -> INICIAL
		LCD_pantalla(PANTALLA_INICIO);		// pantalla a imprimir
		LCD_Hay_cambios();					// CAMBIOS_LCD
	}
}
