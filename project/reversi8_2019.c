/*********************************************************************************************
 * Fichero:	reversi8_2019.c
* Autor1:	Sergio García Esteban	755844
* Autor2:	Irene Fumanal Lacoma	758325
* Descrip:	Codigo del juego reversi
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "reversi8_2019.h"
#include "timer2.h"
#include "pantalla.h"
#include <stdint.h>


/*--- valores y constantes ---*/

enum {// Valores que puede devolver la función patron_volteo())
	NO_HAY_PATRON = 0,
  PATRON_ENCONTRADO = 1
};

const char  NO              = 0;	// candidatas: indica las posiciones a explorar
const char  SI              = 1;	// Se usa para no explorar toda el tablero innecesariamente
const char  CASILLA_OCUPADA = 2;	// Sus posibles valores son NO, SI, CASILLA_OCUPADA

static const char __attribute__ ((aligned (8))) tabla_valor[DIM][DIM] =
{
    {8,2,7,3,3,7,2,8},	// Tabla para decidir el movimiento de la maquina
    {2,1,4,4,4,4,1,2},
    {7,4,6,5,5,6,4,7},
    {3,4,5,0,0,5,4,3},
    {3,4,5,0,0,5,4,3},
    {7,4,6,5,5,6,4,7},
    {2,1,4,4,4,4,1,2},
    {8,2,7,3,3,7,2,8}
};

// Direccion de los 8 desplazamientos posibles
const char vSF[DIM] = {-1,-1, 0, 1, 1, 1, 0,-1};
const char vSC[DIM] = { 0, 1, 1, 1, 0,-1,-1,-1};


//Declaracion funciones diseñadas en practica 1
extern int patron_volteo_arm_arm(char tablero[][8], int *longitud,char f, char c, char SF, char SC, char color);
//extern int patron_volteo_arm_c(char tablero[][8], int *longitud,char f, char c, char SF, char SC, char color);


/*--- codigo de funciones ---*/

/*
 * Inicializa candidatas y tablero con los valores de inicio de partida
 */
void init_table(char tablero[][DIM], char candidatas[][DIM] ){
    int i, j;
    for (i = 0; i < DIM; i++){
        for (j = 0; j < DIM; j++)
            tablero[i][j] = CASILLA_VACIA;
    }

    for (i = 3; i < 5; ++i) {
    	for(j = 3; j < 5; ++j) {
    		tablero[i][j] = i == j ? FICHA_BLANCA : FICHA_NEGRA;
    	}
    }

    for (i = 2; i < 6; ++i) {
    	for (j = 2; j < 6; ++j) {
    		if((i>=3) && (i < 5) && (j>=3) && (j<5)) {
    				candidatas[i][j] = CASILLA_OCUPADA;
    		}else{
    			candidatas[i][j] = SI; //CASILLA_LIBRE;
    		}
    	}
    }
}

////////////////////////////////////////////////////////////////////////////////
// Devuelve el contenido de la posición indicadas por la fila y columna actual.
// Además informa si la posición es válida y contiene alguna ficha.
// Esto lo hace por referencia (en *posicion_valida)
// Si devuelve un 0 no es válida o está vacia.
char ficha_valida(char tablero[][DIM], char f, char c, int *posicion_valida){
    char ficha;
    // ficha = tablero[f][c];
    // no puede accederse a tablero[f][c]
    // ya que algún índice puede ser negativo
    if ((f < DIM) && (f >= 0) && (c < DIM) && (c >= 0) && (tablero[f][c] != CASILLA_VACIA)){
        *posicion_valida = 1;
        ficha = tablero[f][c];
    }else{
        *posicion_valida = 0;
        ficha = CASILLA_VACIA;
    }
    return ficha;
}

////////////////////////////////////////////////////////////////////////////////
// La función patrón volteo comprueba si hay que actualizar una determinada direccion,
// busca el patrón de volteo (n fichas del rival seguidas de una ficha del jugador actual)
// en una dirección determinada
// SF y SC son las cantidades a sumar para movernos en la dirección que toque
// color indica el color de la pieza que se acaba de colocar
// la función devuelve PATRON_ENCONTRADO (1) si encuentra patrón y NO_HAY_PATRON (0) en caso contrario
// FA y CA son la fila y columna a analizar
// longitud es un parámetro por referencia. Sirve para saber la longitud del patrón que se está analizando.
//          Se usa para saber cuantas fichas habría que voltear
int patron_volteo(char tablero[][DIM], int *longitud, char FA, char CA, char SF, char SC, char color){
	int posicion_valida; // indica si la posición es valida y contiene una ficha de algún jugador
	char casilla;   // casilla es la casilla que se lee del tablero

	FA = FA + SF;
	CA = CA + SC;
	casilla = ficha_valida(tablero, FA, CA, &posicion_valida);
	while ((posicion_valida == 1) && (casilla != color)){
	// mientras la casilla está en el tablero, no está vací­a,
	// y es del color rival seguimos buscando el patron de volteo
		FA = FA + SF;
		CA = CA + SC;
		*longitud = *longitud + 1;
		casilla = ficha_valida(tablero, FA, CA, &posicion_valida);
	}
    // si la ultima posición era válida y la ficha es del jugador actual,
    // entonces hemos encontrado el patrón
	if ((posicion_valida == 1) && (casilla == color) && (*longitud >0))
		return PATRON_ENCONTRADO; // si hay que voltear una ficha o más hemos encontrado el patrón
	else
		return NO_HAY_PATRON; // si no hay que voltear no hay patrón
}

/*
 * Voltea N fichas desde (FA,CA) en la dirección (SF,SC) con ficha COLOR.
 */
void voltear(char tablero[][DIM], char FA, char CA, char SF, char SC, int n, char color){
    int i;
    for (i = 0; i < n; i++){
        FA = FA + SF;
        CA = CA + SC;
        tablero[FA][CA] = color;
    }
}

/*
 * Comprueba si tras un movimiento (f,c) hay que voltear fichas y las voltea.
 */
int actualizar_tablero(char tablero[][DIM], char f, char c, char color){
    char SF, SC; // cantidades a sumar para movernos en la dirección que toque
    int i, flip, patron;

    // char vSF[DIM] = {-1,-1, 0, 1, 1, 1, 0,-1};
    // char vSC[DIM] = { 0, 1, 1, 1, 0,-1,-1,-1};
    for (i = 0; i < DIM; i++){ // 0 es Norte, 1 NE, 2 E ...
        SF = vSF[i];
        SC = vSC[i];
        // flip: numero de fichas a voltear
        flip = 0;
        unsigned int t1,t2;
        t1=timer2_leer();
        patron = patron_volteo_arm_arm(tablero, &flip, f, c, SF, SC, color);
        t2=timer2_leer();
        LCD_tvolteo(t2-t1);
        //printf("Flip: %d \n", flip);
        if (patron == PATRON_ENCONTRADO ){
            voltear(tablero, f, c, SF, SC, flip, color);
        }
    }
    return 0;
}

/*
 * Recorre el tablero comprobando en cada posición si se puede mover.
 * Entre los posibles movimientos compara posiciones y elige la mejor.
 * Devuelve solución en f y c.
 */
int elegir_mov(char candidatas[][DIM], char tablero[][DIM], char *f, char *c){
    int i, j, k, found;
    int mf = -1; // almacena la fila del mejor movimiento encontrado
    int mc;      // almacena la columna del mejor movimiento encontrado
    char mejor = 0; // almacena el mejor valor encontrado
    int patron, longitud;
    char SF, SC; // cantidades a sumar para movernos en la dirección que toque

    // Recorremos el tablero comprobando dónde podemos mover
    // Comparamos la puntuación de los movimientos encontrados y nos quedamos con el mejor
    for (i=0; i<DIM; i++){
        for (j=0; j<DIM; j++){   // indica en qué casillas quizá se pueda mover
            if (candidatas[i][j] == SI){
                if (tablero[i][j] == CASILLA_VACIA){
                    found = 0;
                    k = 0;

                    // en este bucle comprobamos si es un movimiento válido
                    // (es decir si implica voltear en alguna dirección)
                    while ((found == 0) && (k < DIM)){
                        SF = vSF[k];    // k representa la dirección que miramos
                        SC = vSC[k];    // 1 es norte, 2 NE, 3 E ...

                        // nos dice qué hay que voltear en cada dirección
                        longitud = 0;
                        unsigned int t1,t2;
                        t1=timer2_leer();
                        patron = patron_volteo_arm_arm(tablero, &longitud, i, j, SF, SC, FICHA_BLANCA);
                        t2=timer2_leer();
                        LCD_tvolteo(t2-t1);
                        //  //printf("%d ", patron);
                        if (patron == PATRON_ENCONTRADO){
                            found = 1;
                            if (tabla_valor[i][j] > mejor){
                                mf = i;
                                mc = j;
                                mejor = tabla_valor[i][j];
                            }
                        }
                        k++;
                        // si no hemos encontrado nada probamos con la siguiente dirección
                    }
                }
            }
        }
    }
    *f = (char) mf;
    *c = (char) mc;
    // si no se ha encontrado una posición válida devuelve -1
    return mf;
}
/*
 * Devuelve el número de fichas existentes de cada color en el tablero
 */
void contar(char tablero[][DIM], int *b, int *n){
    int i,j;

    *b = 0;
    *n = 0;

    // recorremos el tablero contando las fichas de cada color
    for (i=0; i<DIM; i++){
        for (j=0; j<DIM; j++){
            if (tablero[i][j] == FICHA_BLANCA){
                (*b)++;
            }else if (tablero[i][j] == FICHA_NEGRA){
                (*n)++;
            }
        }
    }
}
/*
 * Actualiza candidatas tras un movimiento
 */
void actualizar_candidatas(char candidatas[][DIM], char f, char c){
    // donde ya se ha colocado no se puede volver a colocar
    // En las posiciones alrededor sí
    candidatas[f][c] = CASILLA_OCUPADA;
    if (f > 0)
    {
        if (candidatas[f-1][c] != CASILLA_OCUPADA)
            candidatas[f-1][c] = SI;

        if ((c > 0) && (candidatas[f-1][c-1] != CASILLA_OCUPADA))
            candidatas[f-1][c-1] = SI;

        if ((c < 7) && (candidatas[f-1][c+1] != CASILLA_OCUPADA))
            candidatas[f-1][c+1] = SI;
    }
    if (f < 7)
    {
        if (candidatas[f+1][c] != CASILLA_OCUPADA)
            candidatas[f+1][c] = SI;

        if ((c > 0) && (candidatas[f+1][c-1] != CASILLA_OCUPADA))
            candidatas[f+1][c-1] = SI;

        if ((c < 7) && (candidatas[f+1][c+1] != CASILLA_OCUPADA))
            candidatas[f+1][c+1] = SI;
    }
    if ((c > 0) && (candidatas[f][c-1] != CASILLA_OCUPADA))
        candidatas[f][c-1] = SI;

    if ((c < 7) && (candidatas[f][c+1] != CASILLA_OCUPADA))
        candidatas[f][c+1] = SI;
}

/*
 * Comprueba si (f,c) es un movimiento válido
 */
int movimiento_valido(char tablero[][DIM], char f, char c){
    char SF, SC; // cantidades a sumar para movernos en la dirección que toque
    int i=0, patron=0, longi;

    // char vSF[DIM] = {-1,-1, 0, 1, 1, 1, 0,-1};
    // char vSC[DIM] = { 0, 1, 1, 1, 0,-1,-1,-1};

    while ( i < DIM & patron==0){ // 0 es Norte, 1 NE, 2 E ...
        SF = vSF[i];
        SC = vSC[i];

        unsigned int t1,t2;
        t1=timer2_leer();
        patron = patron_volteo_arm_arm(tablero, &longi, f, c, SF, SC, FICHA_NEGRA);
        t2=timer2_leer();
        LCD_tvolteo(t2-t1);

        i++;
    }
    return patron;
}

/*
 * Funcion encargada de realizar la jugada correspondiente con una
 * ficha y columna determinadas
 */
int reversi_procesar(char tablero[][DIM], char candidatas[][DIM], char fila, char columna){
	unsigned int t1,t2;
	t1=timer2_leer();
		int done;     		// la máquina ha conseguido mover o no
		int move = 0; 		// el humano ha conseguido mover o no
		int blancas, negras;// número de fichas de cada color
		int fin = 0;  		// fin vale 1 si no hay casillas vacias
		char f, c;    		// fila y columna elegidas por la máquina para su movimiento

		//movimiento del humano
	    if (((fila) != DIM) && ((columna) != DIM)){
	    	if((tablero[fila][columna]) == CASILLA_VACIA){
	    					// Si es posicion correcta y está libre escribimos
	    					// ficha en tablero, volteamos y actualizamos candidatas.
	    		tablero[fila][columna] = FICHA_NEGRA;
	    		actualizar_tablero(tablero, fila, columna, FICHA_NEGRA);
	    		actualizar_candidatas(candidatas, fila, columna);
	    		move = 1;
	    	}
	    }

	    //movimiento de la maquina
	    done = elegir_mov(candidatas, tablero, &f, &c);
	    if (done == -1){ 	// La maquina no puede mover
	        contar(tablero, &blancas, &negras);
	        if (blancas+negras == DIM*DIM | move==0){
	            fin = 1;	// Si no puede mover la máquina, comprobamos fin de partida,
	        }				// es fin si el humano no ha movido o si el tablero está lleno.
	    }else{
	        tablero[f][c] = FICHA_BLANCA;
	        actualizar_tablero(tablero, f, c, FICHA_BLANCA);
	        actualizar_candidatas(candidatas, f, c);
	    }
	    t2=timer2_leer();	// El tiempo de cálculo es el tiempo de ejecución de reversi_procesar();
	    LCD_tcalculo(t2-t1);
	    return fin;
}


