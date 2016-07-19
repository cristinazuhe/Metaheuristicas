#include <stdio.h>
#include <stdlib.h>
#include "bmb.h"
#include <stdbool.h>

void alg_Busqueda_Multiarranque_Basica(int n, int **mat1, int **mat2, int *solucion, int *costo){
    int contador = 0;
    //Creo solución auxiliar para ir generando soluciones a partir de la solucion inicial aleatoria
    int *solucion_aux = (int *)malloc(n*sizeof(int));
    for (int i=0; i<n; i++){
  	solucion_aux[i]=solucion[i];
    }
    int coste = costeSolucion(solucion,n);
    int mejorcosto = coste;

    while (contador < 25){
        Set_random(550055);      //Vamos cambiando la semilla y obtenemos distintos aleatorios
       	permutar_Solucion_n(solucion_aux,n); 	//Genero la solución incial

        //Aplico Búsqueda local
        alg_Busqueda_Local(n, mat1, mat2, solucion_aux, &coste, false);
        //Si el coste mejora en la solución auxilar, actualizo la solucion global
        if (coste < mejorcosto){
            mejorcosto = coste;
            for(int i=0;i<n;i++){
                solucion[i]=solucion_aux[i];
            }
        }
        contador++;
    }
    (*costo) = costeSolucion(solucion,n);
}
