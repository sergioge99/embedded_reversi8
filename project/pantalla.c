/*********************************************************************************************
* Fichero:		pantalla.c
* Autor1:		Sergio García Esteban	755844
* Autor2:		Irene Fumanal Lacoma	758325
* Descrip:		Funciones de pantalla.
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "pantalla.h"
#include "lcd.h"
#include "itoa.h"
#include "timer2.h"
#include "Bmp.h"

// Bitmaps de las fichas negras, blancas y grises
extern STRU_BITMAP Stru_Bitmap_fichaNegra;
extern STRU_BITMAP Stru_Bitmap_fichaBlanca;
extern STRU_BITMAP Stru_Bitmap_fichaGris;

/*--- valores y constantes ---*/
enum { CONTADOR_COMPARADOR=134 }; //134 segundos es el máximo tiempo que mide timer2

/*--- variables globales ---*/
static int pantalla=PANTALLA_CALIBRADO;
static int DMA_ready=1, Hay_cambios=1;
static int primeraImpresion=1;
static int fMov=0, cMov=0;
static int oldfMov=0, oldcMov=0;
static char underGris=0;
static char toblerone[DIM][DIM]= {
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0}
};
static char cambios[DIM][DIM]= {
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0}
};
static int ncambios=0;
static int tcalculos=0, tvolteos=0, nvolteos=0;
static int ttotal=0, tinicio_juego=0, contador_multiplicador=0, tanterior=0;
static int esquina=0;
static int negras=0, blancas=0;

/*--- codigo de las funciones ---*/
/*
 * Pantalla fin de partida
 */
void Pantalla_final(){
	if(primeraImpresion==1){
		primeraImpresion=0;
		Lcd_Clr();

		// Leyenda fin del juego
		static char a[]="Fin del juego";
		Lcd_DspAscII8x16(118,70,BLACK,a);

		// Resultado negras y blancas
		static char snum[10];
		static char b[]="Negras";
		Lcd_DspAscII6x8(84,92,BLACK,b);
		itoa(negras,snum);
		Lcd_DspAscII8x16(96,102,BLACK,snum);
		static char c[]="Blancas";
		Lcd_DspAscII6x8(194,92,BLACK,c);
		itoa(blancas,snum);
		Lcd_DspAscII8x16(212,102,BLACK,snum);

		// Boton jugar de nuevo
		Lcd_Draw_Box(100, 130, 220, 162, 15);
		static char d[]="Volver a jugar";
		Lcd_DspAscII6x8(118,138,BLACK,d);

		// Boton pantalla inicial
		Lcd_Draw_Box(100, 170, 220, 202, 15);
		static char e[]="Salir";
		Lcd_DspAscII6x8(145,178,BLACK,e);
	}
}
/*
 * Pantalla de calibrado
 * Dibuja un cuadrado en la esquina que corresponda
 */
void Pantalla_calibrar(){
	if(primeraImpresion==1){
		primeraImpresion=0;
		Lcd_Clr();

		// Leyenda Haz click
		static char a[]="Haz click en el cuadrado";
		Lcd_DspAscII6x8(88,116,BLACK,a);

		// Cuadrado arriba izq
		Lcd_Draw_Box(0, 0, 20, 20, 15);
	}else{
		if(esquina==1){
			// Cuadrado arriba der (eliminamos anterior)
			LcdClrRect(0, 0, 20, 20, 0);
			Lcd_Draw_Box(299, 0, 319, 20, 15);
		}else if(esquina==2){
			// Cuadrado abajo izq (eliminamos anterior)
			LcdClrRect(299, 0, 319, 20, 0);
			Lcd_Draw_Box(0, 219, 20, 239, 15);
		}else if(esquina==3){
			// Cuadrado abajo der (eliminamos anterior)
			LcdClrRect(0, 219, 20, 239, 0);
			Lcd_Draw_Box(299, 219, 319, 239, 15);
		}
	}
}
/*
 * Pantalla inicial
 */
void Pantalla_inicial(){
	if(primeraImpresion==1){
		primeraImpresion=0;
		Lcd_Clr();

		// Titulo y leyenda Toque para jugar
		static char a[]="Reversi 8";
		Lcd_DspAscII8x16(124,90,BLACK,a);
		static char b[]="Toque para jugar";
		Lcd_DspAscII8x16(96,122,BLACK,b);

		// Boton calibrar
		Lcd_Draw_Box(236, 2, 317, 50, 15);
		static char c[]="Calibrar";
		Lcd_DspAscII6x8(246,12,BLACK,c);
		static char d[]="pantalla";
		Lcd_DspAscII6x8(246,30,BLACK,d);

		// Instrucciones
		static char e[]="Instrucciones:";
		Lcd_DspAscII6x8(0,200,BLACK,e);
		static char f[]="Pulse los botones para seleccionar fila y columna";
		Lcd_DspAscII6x8(0,210,BLACK,f);
		static char g[]="Pulse la pantalla para efectuar el movimiento";
		Lcd_DspAscII6x8(0,220,BLACK,g);
		static char h[]=" seleccionado";
		Lcd_DspAscII6x8(0,230,BLACK,h);
	}
}
/*
 * Pantalla de juego
 */
void Pantalla_juego(){
	int i,j;
	static char snum[10];
	if(primeraImpresion==1){
		primeraImpresion=0;
		Lcd_Clr();
		// Inicializamos tiempos de la partida
		tcalculos=0, tvolteos=0, nvolteos=0;
		contador_multiplicador=0, tanterior=0, ttotal=0;
		tinicio_juego=timer2_leer()/1000000;
		contador_multiplicador=0;

		// Tiempos
		static char a[]="Tiempo total";
		Lcd_DspAscII6x8(0,9,BLACK,a);
		static char b[]="Tiempo calculos";
		Lcd_DspAscII6x8(0,45,BLACK,b);
		static char c[]="Tiempo volteos";
		Lcd_DspAscII6x8(0,81,BLACK,c);
		static char d[]="Numero volteos";
		Lcd_DspAscII6x8(0,117,BLACK,d);
		itoa(ttotal,snum);
		Lcd_DspAscII6x8(0,21,BLACK,snum);
		itoa(tcalculos,snum);
		Lcd_DspAscII6x8(0,57,BLACK,snum);
		itoa(tvolteos,snum);
		Lcd_DspAscII6x8(0,93,BLACK,snum);
		itoa(nvolteos,snum);
		Lcd_DspAscII6x8(0,129,BLACK,snum);

		//Boton abandonar
		Lcd_Draw_Box(0,211,117,239,BLACK);
		static char e[]="Abandonar";
		Lcd_DspAscII6x8(31,221,BLACK,e);

		//Boton jugar
		Lcd_Draw_Box(119,211,319,239,BLACK);
		static char f[]="Jugar";
		Lcd_DspAscII6x8(204,221,BLACK,f);

		//Numeros filas y columnas
		for (i = 0; i < DIM; i++){
			itoa(i,snum);
			Lcd_DspAscII6x8(128+25*i,0,BLACK,snum);
		}
		for (i = 0; i < DIM; i++){
			itoa(i,snum);
			Lcd_DspAscII6x8(112,17+25*i,BLACK,snum);
		}

		//Recuadro tablero
		Lcd_Draw_Box(119, 9, 319, 209, 15);
		//Lineas horizontales tablero
		Lcd_Draw_HLine(119, 319 , 34 , 15, 1);
		Lcd_Draw_HLine(119, 319 , 59 , 15, 1);
		Lcd_Draw_HLine(119, 319 , 84 , 15, 1);
		Lcd_Draw_HLine(119, 319 , 109 , 15, 1);
		Lcd_Draw_HLine(119, 319 , 134 , 15, 1);
		Lcd_Draw_HLine(119, 319 , 159 , 15, 1);
		Lcd_Draw_HLine(119, 319 , 184 , 15, 1);
		//Lineas verticales tablero
		Lcd_Draw_VLine(9,  209, 144, 15, 1);
		Lcd_Draw_VLine(9,  209, 169, 15, 1);
		Lcd_Draw_VLine(9,  209, 194, 15, 1);
		Lcd_Draw_VLine(9,  209, 219, 15, 1);
		Lcd_Draw_VLine(9,  209, 244, 15, 1);
		Lcd_Draw_VLine(9,  209, 269, 15, 1);
		Lcd_Draw_VLine(9,  209, 294, 15, 1);

		//FICHAS - matriz de cambios del tablero
		for (i = 0; i < DIM; i++){
			for (j = 0; j < DIM; j++){
				if(cambios[i][j] == 1){
					BitmapView(120+(j*25),10+(i*25),Stru_Bitmap_fichaBlanca);
					cambios[i][j]=0;
					ncambios--;
				}else if(cambios[i][j] == 2){
					BitmapView(120+(j*25),10+(i*25),Stru_Bitmap_fichaNegra);
					cambios[i][j]=0;
					ncambios--;
				}
			}
		}

		//FICHA GRIS
		underGris=toblerone[fMov][cMov];	// colocamos la ficha gris
		oldfMov=fMov;						// guardamos su posicion
		oldcMov=cMov;						// guardamos la ficha de debajo
		BitmapView(120+(cMov*25),10+(fMov*25),Stru_Bitmap_fichaGris);

	}else{
		//Calculamos ttotal
		unsigned int tactual=timer2_leer()/1000000;
		if(tactual < tanterior){
			contador_multiplicador++;
		}
		tanterior=tactual;
		ttotal=(CONTADOR_COMPARADOR*contador_multiplicador)+tactual-tinicio_juego;

		//Tiempos
		static char snum[10];
		LcdClrRect(0, 21, 100, 37, 0);		// eliminamos numero anterior
		itoa(ttotal,snum);					// convertimos int a char
		Lcd_DspAscII8x16(0,21,BLACK,snum);	// escribimos numero
		LcdClrRect(0, 57, 100, 73, 0);
		itoa(tcalculos,snum);
		Lcd_DspAscII8x16(0,57,BLACK,snum);
		LcdClrRect(0, 93, 100, 109, 0);
		itoa(tvolteos,snum);
		Lcd_DspAscII8x16(0,93,BLACK,snum);
		LcdClrRect(0, 129, 100,145, 0);
		itoa(nvolteos,snum);
		Lcd_DspAscII8x16(0,129,BLACK,snum);

		//UNDER FICHA GRIS
		if((fMov!=oldfMov)|(cMov!=oldcMov)){	// Restauramos la ficha debajo de la ficha gris
			LcdClrRect(120+(oldcMov*25),10+(oldfMov*25),120+23+(oldcMov*25),10+23+(oldfMov*25), 0);
			if(underGris==1){
				BitmapView(120+(oldcMov*25),10+(oldfMov*25),Stru_Bitmap_fichaBlanca);
			}else if(underGris==2){
				BitmapView(120+(oldcMov*25),10+(oldfMov*25),Stru_Bitmap_fichaNegra);
			}
		}

		//FICHAS - matriz de cambios del tablero
		if(ncambios>0){
			for (i = 0; i < DIM; i++){
				for (j = 0; j < DIM; j++){
					if(cambios[i][j] == 1){
						LcdClrRect(120+(j*25),10+(i*25),120+23+(j*25),10+23+(i*25), 0);
						BitmapView(120+(j*25),10+(i*25),Stru_Bitmap_fichaBlanca);
						cambios[i][j]=0;
						ncambios--;
					}else if(cambios[i][j] == 2){
						LcdClrRect(120+(j*25),10+(i*25),120+23+(j*25),10+23+(i*25), 0);
						BitmapView(120+(j*25),10+(i*25),Stru_Bitmap_fichaNegra);
						cambios[i][j]=0;
						ncambios--;
					}
				}
			}
		}

		//FICHA GRIS
		if((fMov!=oldfMov)|(cMov!=oldcMov)){
			BitmapView(120+(cMov*25),10+(fMov*25),Stru_Bitmap_fichaGris);
			oldfMov=fMov;						// colocamos la ficha gris,
			oldcMov=cMov;						// guardamos su posicion,
			underGris=toblerone[fMov][cMov];	// guardamos la ficha de debajo
		}
	}
}

/*
 * Imprime la pantalla actual si es necesario
 */
void LCD_imprimir(){
	if((DMA_ready==1) & (Hay_cambios==1)){
		DMA_ready=0;
		Hay_cambios=0;

		if(pantalla==PANTALLA_INICIO){		// Saltamos a las funciones de dibujado
			Pantalla_inicial();				// según la pantalla actual.
		}else if(pantalla==PANTALLA_JUEGO){
			Pantalla_juego();
		}else if(pantalla==PANTALLA_FINAL){
			Pantalla_final();
		}else if(pantalla==PANTALLA_CALIBRADO){
			Pantalla_calibrar();
		}
		Lcd_Dma_Trans();					// Iniciamos transferencia.
	}
}
/*
 * Indica pantalla a imprimir
 */
void LCD_pantalla(Pantalla p){
	pantalla=p;
	primeraImpresion=1;
}
/*
 * Indica fila y columna a la que apunta la ficha gris
 */
void LCD_movimiento(int f,int c){
	cMov=c;
	fMov=f;
}
/*
 * Calcula los cambios que se han producido en el tablero.
 * Realiza una copia para calcular los cambios en la siguiente iteración.
 */
void LCD_tablero(char t[DIM][DIM]){
	int i,j;
	if(primeraImpresion==1){
		for (i = 0; i < DIM; i++){
			for (j = 0; j < DIM; j++){
				toblerone[i][j]=0;
				cambios[i][j]=0;
				ncambios=0;
			}
		}
	}
	for (i = 0; i < DIM; i++){
		for (j = 0; j < DIM; j++){
			if(toblerone[i][j]!=t[i][j]){
				toblerone[i][j]=t[i][j];
				cambios[i][j]=t[i][j];
				ncambios++;
			}
		}
	}
}
/*
 * Actualiza tcalculos
 */
void LCD_tcalculo(unsigned int t){
	tcalculos+=t;
}
/*
 * Actualiza tvolteos y nvolteos
 */
void LCD_tvolteo(unsigned int t){
	tvolteos+=t;
	nvolteos++;
}
/*
 * Indica siguiente esquina a calibrar
 */
void LCD_esquina(int e){
	esquina=e;
}
/*
 * Indica resultado al final de la partida
 */
void LCD_fichas(int b, int n){
	blancas=b;
	negras=n;
}
/*
 * Indica que la transferencia dma ha finalizado
 */
void LCD_DMA_ready(){
	DMA_ready=1;
}
/*
 * Indica que hay que ejecutar una transferencia
 */
void LCD_Hay_cambios(){
	Hay_cambios=1;
}
/*
 * Si hay que actualizar el ttotal, pone Hay_cambios a 1
 */
void LCD_Refresca_ttotal(){
	static int contador=0;
	if(pantalla==PANTALLA_JUEGO){
		contador++;
		if(contador==60){	//Esta funcion se ejecuta 60 veces por segundo
			contador=0;
			Hay_cambios=1;
		}
	}
}
