/*********************************************************************************************
* Fichero:	itoa.c
* Autor:
* Descrip: Funcion itoa
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "itoa.h"
#include <string.h>

/*--- codigo de las funciones ---*/

/*********************************************************************************************
* Funcion auxiliar que da la vuelta un string
*********************************************************************************************/
 void reverse(char s[])
 {
     int i, j;
     char c;

     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }
 /*********************************************************************************************
 * Funcion que convierte un entero a cadena de carácteres
 *********************************************************************************************/
void itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}
