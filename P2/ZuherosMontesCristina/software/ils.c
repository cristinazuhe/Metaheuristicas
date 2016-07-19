#include <stdio.h>
#include <stdlib.h>
#include "ils.h"
#include <stdbool.h>

void AplicarMutacion(int *solucion, int n){
	int tam_sublista = n/4;
	int aleatorio = obtener_aleatorio(0,n-1);

	if (n >= 2 && n<=4){
	     	tam_sublista = 2;
        int alea2 = obtener_aleatorio(0,n-1);
        int aux = solucion[aleatorio];
        solucion[aleatorio]= solucion[alea2];
        solucion[alea2]=aux;
	}
	else {
      int posini=0;
        if (aleatorio >= (n-tam_sublista)){
           posini = n - tam_sublista;            //los tam_sublista últimos
        }
        else {
            posini = aleatorio;            //de aleatorio en adelante
        }
        int *solucion_sublis = (int *)malloc(n*sizeof(int));
        for (int i=0; i<tam_sublista; i++){
          solucion_sublis[i]=solucion[posini+i];
        }
        permutar_Solucion_n(solucion_sublis,tam_sublista); 	//Genero la solución incial
        for (int i=0;i<tam_sublista;i++){
            solucion[posini+i]=solucion_sublis[i];
        }
	}

}

void alg_Busqueda_Local_Reiterada(int n, int **mat1, int **mat2, int *solucion, int *costo){

      int contador = 0;
      //Creo solución auxiliar para ir generando soluciones a partir de la solucion inicial aleatoria
			int *solucion_aux = (int *)malloc(n*sizeof(int));
			for (int i=0; i<n; i++){
						solucion_aux[i]=solucion[i];
			}
			int coste = costeSolucion(solucion,n);
			int mejorcosto = coste;

      while (contador < 25){
        if(contador==0){
          Set_random(550055);      //Vamos cambiando la semilla y obtenemos distintos aleatorios
          permutar_Solucion_n(solucion_aux,n); 	//Genero la solución incial
          for (int i=0; i<n; i++){
              solucion[i]=solucion_aux[i];
          }
          mejorcosto = costeSolucion(solucion_aux,n);

        }else{
          AplicarMutacion(solucion_aux,n);
        }
          //Aplico Búsqueda local
          alg_Busqueda_Local(n, mat1, mat2, solucion_aux, &coste, false);
          //Actualizo si es necesario la mejor solución
          if (coste < mejorcosto){
              mejorcosto = coste;
              for(int i=0;i<n;i++){
                  solucion[i]=solucion_aux[i];
              }
          }
          else {
              //vuelvo a la mejor global que es a la que haré la mutación
              for(int i=0;i<n;i++){
                  solucion_aux[i]=solucion[i];
              }
          }
          contador++;
      }
      (*costo) = costeSolucion(solucion,n);
}
