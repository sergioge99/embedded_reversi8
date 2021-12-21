/*********************************************************************************************
* Fichero:		pruebas_patron_volteo.c
* Autor1:	Sergio García Esteban	755844
* Autor2:	Irene Fumanal Lacoma	758325
* Descrip:		Funciones de debugg de las funciones de patron_volteo.
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "pruebas_patron_volteo.h"
#include "reversi8_2019.h"
#include "timer2.h"

extern int patron_volteo_arm_arm(char tablero[][8], int *longitud,char f, char c, char SF, char SC, char color);
extern int patron_volteo_arm_c(char tablero[][8], int *longitud,char f, char c, char SF, char SC, char color);


/*--- variables globales del módulo ---*/
static const char vSF[DIM] = {-1,-1, 0, 1, 1, 1, 0,-1};
static const char vSC[DIM] = { 0, 1, 1, 1, 0,-1,-1,-1};


/*
 * Compara los resultados de las 3 funciones patron_volteo y tambien las longitudes.
 * Si no son iguales entra en bucle infinito.
 */
int patron_volteo_test(char tablero[][DIM], int *longitud, char FA, char CA, char SF, char SC, char color){
	int sol_1,sol_2,sol_3;		//soluciones: PATRON_ENCONTRADO, NO_HAY_PATRON
	int longitud_1=*longitud;	//valor longitud con el que trabajará la primera función
	int longitud_2=*longitud;
	int longitud_3=*longitud;
	int *plong_1=&longitud_1;	//puntero longitud a pasar por parametro a la primera función
	int *plong_2=&longitud_2;
	int *plong_3=&longitud_3;
	sol_1=patron_volteo(tablero,plong_1,FA,CA,SF,SC,color);
	sol_2=patron_volteo_arm_c(tablero,plong_2,FA,CA,SF,SC,color);
	sol_3=patron_volteo_arm_arm(tablero,plong_3,FA,CA,SF,SC,color);

	//comprobamos si coinciden solucion y longitud de las 3
	if(sol_1!=sol_2||sol_1!=sol_3||longitud_1!=longitud_2||longitud_1!=longitud_3){
		while(1);
	}
	return sol_3;
}

/*
 * Utiliza los tableros t_1,t_2 y t_3  para comprobar el funcionamiento
 * utilizando patron_volteo_test().
 */
void pruebas_patron_volteo(){
	//Tablero prueba 1
	static char __attribute__ ((aligned (8))) t_1[DIM][DIM] = {
				{2,0,0,2,0,0,2,0},
			    {0,1,0,1,0,1,0,0},
			    {0,0,1,1,1,0,0,0},
			    {2,1,1,2,1,1,1,2},
			    {0,0,1,1,1,0,0,0},
			    {0,1,0,1,0,1,0,0},
			    {2,0,0,1,0,0,1,0},
			    {0,0,0,2,0,0,0,2}
	};
	//Tablero prueba 2
	static char __attribute__ ((aligned (8))) t_2[DIM][DIM] = {
				{2,0,0,1,0,0,0,0},
			    {0,0,0,1,0,2,0,0},
			    {0,0,1,1,1,0,0,0},
			    {2,1,0,2,1,0,1,2},
			    {0,0,0,0,2,0,0,0},
			    {0,2,0,0,0,1,0,0},
			    {0,0,0,0,0,0,2,0},
			    {0,0,0,0,0,0,0,0}
	};
	//Tablero prueba 3
	static char __attribute__ ((aligned (8))) t_3[DIM][DIM] = {
				{2,1,1,1,1,1,1,1},
			    {1,1,0,0,0,0,0,0},
			    {1,0,1,0,0,0,0,0},
			    {1,0,0,1,0,0,0,0},
			    {1,0,0,0,1,0,0,0},
			    {1,0,0,0,0,1,0,0},
			    {1,0,0,0,0,0,1,0},
			    {2,0,0,0,0,0,0,2}
	};

	int i,lon;
	char SF,SC,f,c,color;
	f=3;
	c=3;
	color=2;
	for (i = 0; i < DIM; i++){
		lon=0;
		SF = vSF[i];
		SC = vSC[i];
		patron_volteo_test(t_1,&lon,f,c,SF,SC,color);
	}
	for (i = 0; i < DIM; i++){
		lon=0;
		SF = vSF[i];
		SC = vSC[i];
		patron_volteo_test(t_2,&lon,f,c,SF,SC,color);
	}
	f=0;
	c=0;
	for (i = 0; i < DIM; i++){
		lon=0;
		SF = vSF[i];
		SC = vSC[i];
		patron_volteo_test(t_3,&lon,f,c,SF,SC,color);
	}
}


/*
 * Utilizada para medir los tiempos de las 3 funciones patron_volteo.
 * Realizamos la medida individual de cada una de las 8 posibles posiciones incluidas en
 * los vectores:vSF y vSC, y también la medida de las 8 en
 * bucle (descomentar lineas)
 */
void pruebas_tiempo_patron_volteo(){
	//Tablero Medicion de tiempos (Fila 2 Columna 3)
	static char __attribute__ ((aligned (8))) t_t[DIM][DIM] = {
				{0,0,0,0,0,0,0,0},
			    {0,0,0,0,0,0,0,0},
			    {0,0,0,2,0,0,0,0},
			    {0,0,0,1,2,0,0,0},
			    {0,0,0,2,1,0,0,0},
			    {0,0,0,0,0,0,0,0},
			    {0,0,0,0,0,0,0,0},
			    {0,0,0,0,0,0,0,0}
	};
	unsigned int t1=0;
	unsigned int t2=0;
	unsigned int t3=0;
	unsigned int bruto1[DIM];
	unsigned int bruto2[DIM];
	unsigned int bruto3[DIM];
	unsigned int total;
	int i,l;
	char f,c,color;
	f=2;
	c=3;
	color=2;
	timer2_inicializar();


	//timer2_empezar();
	for (i = 0; i < DIM; i++){
		l=0;
		timer2_empezar();
		patron_volteo(t_t,&l,f,c,vSF[i],vSC[i],color);
		bruto1[i] = timer2_parar();
		t1+=bruto1[i];
	}
	//t1 = timer2_parar();

	//timer2_empezar();
	for (i = 0; i < DIM; i++){
		l=0;
		timer2_empezar();
		patron_volteo_arm_c(t_t,&l,f,c,vSF[i],vSC[i],color);
		bruto2[i] = timer2_parar();
		t2+=bruto2[i];
	}
	//t2 = timer2_parar();

	//timer2_empezar();
	for (i = 0; i < DIM; i++){
		l=0;
		timer2_empezar();
		patron_volteo_arm_arm(t_t,&l,f,c,vSF[i],vSC[i],color);
		bruto3[i] = timer2_parar();
		t3+=bruto3[i];
	}
	//t3 = timer2_parar();
	total=t1+t2+t3;	// PONER BREAKPOINT AQUI

}



