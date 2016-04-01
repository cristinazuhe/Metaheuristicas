#include <stdio.h>
#include <stdlib.h>
#include "greedy.h"
#include <stdbool.h>

//Algoritmo Greedy
void alg_Greedy(int n, int **mat1, int **mat2, int *solucion){
	//Calculo los potenciales de flujo y de distancia
	int potencial_f[n], potencial_d[n];
	for (int i=0; i<n; i++){
		potencial_f[i]=0;
		potencial_d[i]=0;
		for (int j=0; j<n; j++){  //Suma de las filas de las matrices
			potencial_f[i] += mat1[i][j];
			potencial_d[i] += mat2[i][j];
		}
	}

	int sol_unidad[n];        //Vector para almacenar las unidades correctamente
	int sol_localizacion[n];  //Vector para almacenas las localidades correctamente
	bool unidad_asociada,localizacion_asociada; //Evitamos duplicar unidades/localidades
	for (int i=0; i<n; i++){
		int unidad=0, localizacion=0;
		for (int j=1; j<n; j++){
			if (potencial_f[unidad] < potencial_f[j]){ //Me quedo con la unidad con mayor flujo...
				unidad_asociada=false;
				for (int h=0; h<i && !unidad_asociada; h++){  //...que no esté ya asignada
					if (j==sol_unidad[h])
						unidad_asociada=true;
				}
				if(!unidad_asociada)  //Si no está ya asignada, la consideramos como la mejor unidad
					unidad=j;
			}
			if (potencial_d[localizacion] > potencial_d[j]){ //Le asocio la menor distancia...
				localizacion_asociada=false;
				for (int h=0; h<i && !localizacion_asociada; h++){  //...que no esté ya asociada
					if (j==sol_localizacion[h])
						localizacion_asociada=true;
				}
				if(!localizacion_asociada)  //Si no está ya asociada, la consideramos como la mejor localizacion
					localizacion=j;
			}
		}
		//Añado la mejor unidad y su localización al vector de unidades y localizaciones solución
		sol_unidad[i]=unidad;
		sol_localizacion[i]=localizacion;
		potencial_f[unidad]=-9999999;
		potencial_d[localizacion]=9999999;
	}
	//asigno las unidades con sus localizaciones obtenidas al vector solución
	for(int i=0; i<n; i++){
		solucion[sol_unidad[i]]=sol_localizacion[i];
	}
}
