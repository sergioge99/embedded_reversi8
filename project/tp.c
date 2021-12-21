/*********************************************************************************************
* Fichero:	tp.c
* Autor1:	Sergio García Esteban	755844
* Autor2:	Irene Fumanal Lacoma	758325
* Descrip:	Funciones de manejo de la pantalla tactil
* Version:
*********************************************************************************************/

/*--- include files ---*/
#include "tp.h"
#include "cola_depuracion.h"
#include "44blib.h"
#include "timer2.h"
#include <stdint.h>

/*--- variables globales ---*/
static int calibrar=4,primerClick=1,primeraInterrupcion=1;
static unsigned int  Xmax, Ymax, Xmin, Ymin; //Valores maximos de medicion


/* declaración de función que es rutina de servicio de interrupción*/
void touchpad_ISR() __attribute__((interrupt("IRQ")));

/*********************************************************************************************
* Calibra la pantalla, establece nuevos maximos de mediciones.
*********************************************************************************************/
void touchpad_calibrar(ULONG tx, ULONG ty){
    if(primerClick == 1){
    	primerClick=0;
    	Xmax = tx;				// En la primera pulsación asignamos sin comparar
    	Xmin = tx;
    	Ymax = ty;
    	Ymin = ty;
    }else{
		if(tx < Xmin){			// En las siguientes, comprobamos que es mejor maximo/minimo
			Xmin=tx;
		}else if(tx > Xmax){
			Xmax=tx;
		}
		if(ty > Ymax){
			Ymax=ty;
		}else if(ty < Ymax){
			Ymin=ty;
		}
	}
    calibrar--;
}

/*********************************************************************************************
* ISR pantalla tactil
********************************************************************************************/
void touchpad_ISR(){
	if(primeraInterrupcion==0){
		rINTMSK |= BIT_EINT2;
		static int   i;
		//char fail = 0;	//NO SE USA
		static ULONG tmp;
		static ULONG Pt[9];

//---------------- * AD CONVERSOR X-POSITION * ---------------------

		// TSPX(GPE4_Q4(+)) TSPY(GPE5_Q3(-)) TSMY(GPE6_Q2(+)) TSMX(GPE7_Q1(-))
		//       0               1                 1                0
		rPDATE=0x68;
		rADCCON=0x1<<2;					// AIN1
		DelayTime(1000);                // delay to set up the next channel

		for( i=0; i<8; i++ )
		{
			rADCCON |= 0x1;				// Start X-position A/D conversion
			while( rADCCON & 0x1 );		// Check if Enable_start is low
			while( !(rADCCON & 0x40) );	// Check ECFLG
			Pt[i] = (0x3ff&rADCDAT);
		}
		// read X-position AVERAGE value
		Pt[8] = (Pt[0]+Pt[1]+Pt[2]+Pt[3]+Pt[4]+Pt[5]+Pt[6]+Pt[7])/8;

		tmp = Pt[8];

//---------------- * AD CONVERSOR Y-POSITION * ---------------------

		// TSPX(GPE4_Q4(-)) TSPY(GPE5_Q3(+)) TSMY(GPE6_Q2(-)) TSMX(GPE7_Q1(+))
		//       1               0                 0                1
		rPDATE=0x98;
		rADCCON=0x0<<2;		        	// AIN0

		DelayTime(1000);                // delay to set up the next channel
		for( i=0; i<8; i++ )
		{
			rADCCON |= 0x1;             // Start Y-position conversion
			while( rADCCON & 0x1 );     // Check if Enable_start is low
			while( !(rADCCON & 0x40) ); // Check ECFLG
			Pt[i] = (0x3ff&rADCDAT);
		}
		// read Y-position AVERAGE value
		Pt[8] = (Pt[0]+Pt[1]+Pt[2]+Pt[3]+Pt[4]+Pt[5]+Pt[6]+Pt[7])/8;
//-------------------------------------------------------------------------------------
	
		/* --- COORDENADAS --- */

		if(calibrar==0){
			if(tmp < Xmin){		//si estan fuera de los maximos, asignamos el maximo
				tmp = Xmin;
			}else if(tmp > Xmax){
				tmp = Xmax;
			}else if(Pt[8] < Ymin){
				Pt[8] = Ymin;
			}else if(Pt[8] > Ymax){
				Pt[8] = Ymax;
			}
	
			tmp = 320*(tmp - Xmin)/(Xmax - Xmin);   	// calculamos coordenadas en pixeles
			Pt[8] = 240*(Pt[8] - Xmin)/(Ymax - Ymin);
	
			uint32_t auxData = tmp & (0x00000fff);		// añadimos las coordenadas a la info del evento
			auxData |= ( (Pt[8] & (0x00000fff))<<12 );	// auxData=00YYYXXX
			push_debug(ev_tactil,auxData);				// guardamos evento


		}else{	// calibrar > 0
			push_debug(ev_tactil,0);	//si calibra, evento con coordenadas 0,0
			touchpad_calibrar(tmp,Pt[8]);
		}

		rPDATE = 0xb8;                  // should be enabled
		DelayTime(3000);                // delay to set up the next channel
	}else{
		primeraInterrupcion=0;	//SI ES LA PRIMERA, PONEMOS A 0 Y LIMPIAMOS PENDING
	}
    rI_ISPC = BIT_EINT2;            	// clear pending_bit
}

/*********************************************************************************************
* Inicialización de la pantalla táctil
*********************************************************************************************/
void touchpad_init()
{
    rI_ISPC |= BIT_EINT2;           	// clear pending_bit
    pISR_EINT2=(unsigned)touchpad_ISR;	// set interrupt handler

    rPCONG  |= 0x0030;		// Establece la funcion de los pines (EINT2)
    rPUPG   |= 0x04;		// Deshabilita el "pull up" de los pines (EINT2)
    rEXTINT |= 0x00000200;	// Lineas de int. como de flanco de bajada (EINT2)
	

	// TSPX(GPE4_Q4(-)) TSPY(GPE5_Q3(-)) TSMY(GPE6_Q2(-)) TSMX(GPE7_Q1(+)) 
	//          1               1                0                 1

    rPDATE = 0xb8;                  // should be enabled
    DelayTime(100); 

    rCLKCON |= 0x1000;				// enable A/D clock
    rADCPSR = 0x13;					// A/D prescaler = 19+1

    //Xmax = 750;    Xmin = 200;	// calibramos al inicio
    //Ymax = 620;    Ymin = 120;

    rI_ISPC |= BIT_EINT2;           // clear pending_bit
    rINTMSK &= ~(BIT_EINT2);		// habilitamos interrupciones

}

/*********************************************************************************************
* Activa el calibrado en las siguientes Ncal pulsaciones
*********************************************************************************************/
void touchpad_setCalibrar(int Ncal){
	calibrar=Ncal;				// numero de pulsaciones utilizadas para calibrado.
	primerClick=1;
}

/*********************************************************************************************
* Reactiva las interrupciones de la pantalla tactil
*********************************************************************************************/
void touchpad_reactivar(){
	rI_ISPC   |= BIT_EINT2;		// borra el bit pendiente en INTPND.
	rINTMSK &= ~(BIT_EINT2);	// activar interrupciones.
}

/*********************************************************************************************
* Devuelve 1 si el touchpad esta pulsado
*********************************************************************************************/
int touchpad_estado(){
	int data=rPDATG;			// BIT 2 a 1 si touchpad pulsado.
	if( (data & 0x04) == 0 ){	// comprobamos BIT 2.
		return 1;
	}else{
		return 0;
	}
}

/*********************************************************************************************
* Desactiva la pantalla tactil
*********************************************************************************************/
void touchpad_close(){
	rINTMSK |= BIT_EINT2;
	pISR_EINT2 = (int)NULL;
}



