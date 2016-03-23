#include <stdio.h>
#include "random.h"
/////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////PARA EL GENERADOR ALEATORIO//////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
unsigned long Seed = 0L;
#define MASK 2147483647
#define PRIME 65539
#define SCALE 0.4656612875e-9

/* Inicializa la semilla al valor x.*/
void Set_random (unsigned long x){
    Seed = (unsigned long) x;
}

/* Devuelve el valor actual de la semilla */
unsigned long Get_random (void){
    return Seed;
}

/* Genera un numero aleatorio real en el intervalo [0,1[ */
float Rand(){
    return (( Seed = ( (Seed * PRIME) & MASK) ) * SCALE );
}

/*Función para generar un número aleatorio*/
int obtener_aleatorio(int min, int max){
    return (min + (max-(min)+1) * Rand());
}
