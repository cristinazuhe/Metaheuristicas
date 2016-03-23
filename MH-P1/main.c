#include <stdio.h>
#include <stdlib.h>
#include "lectura.h"
#include "greedy.h"
#include "timer.h"
#include "busqueda_local.h"

int **mat1, **mat2, *sol;

void reservarEspacio(int n){
	sol = (int *)malloc(n*sizeof(int));
	mat1 = (int **)malloc(n*sizeof(int*));
	mat2 = (int **)malloc(n*sizeof(int*));
	for (int i=0; i<n; i++){
		mat1[i] = (int*)malloc(n*sizeof(int));
		mat2[i] = (int*)malloc(n*sizeof(int));
	}
}

int costeSolucion(int *solucion, int n){
	int micoste = 0;
	for (int i=0; i<n; i++){
		for (int j=0; j<n; j++){
			if (i!=j){
				micoste += mat1[i][j]*(mat2[ solucion[i]][ solucion[j]]);
			}
		}
	}
	return micoste;
}


void DarNombres(char * nombres[]){
    nombres[0] = "datos/chr20a.dat";
    nombres[1] = "datos/chr20c.dat";
    nombres[2] = "datos/chr22b.dat";
    nombres[3] = "datos/chr25a.dat";
    nombres[4] = "datos/esc32a.dat";
    nombres[5] = "datos/esc64a.dat";
    nombres[6] = "datos/esc128.dat";
    nombres[7] = "datos/kra32.dat";
    nombres[8] = "datos/lipa90a.dat";
    nombres[9] = "datos/sko42.dat";
    nombres[10] = "datos/sko49.dat";
    nombres[11] = "datos/sko81.dat";
    nombres[12] = "datos/sko90.dat";
    nombres[13] = "datos/sko100f.dat";
    nombres[14] = "datos/tai64c.dat";
    nombres[15] = "datos/tai80a.dat";
    nombres[16] = "datos/tai100a.dat";
    nombres[17] = "datos/tai150b.dat";
    nombres[18] = "datos/tai256c.dat";
    nombres[19] = "datos/tho150.dat";
}

void mostrarMatrices(int n){
  int i,j;
	for (i=0; i<n; i++){
    	for (j=0; j<n; j++){
    		printf("%d ", mat1[i][j]);
    	}
    	printf("\n");
  }
  printf("\n");
	for (i=0; i<n; i++){
    	for (j=0; j<n; j++){
    		printf("%d ", mat2[i][j]);
    	}
	printf("\n");
  }
}



int main(int argc, char * argv[])
{
	int n = leerNumeroDatos(argv[1]);
	if (n > 0) {
		reservarEspacio(n);
		obtenerMatrices(argv[1], mat1, mat2);
    double tiempo;

		//Aplico Greedy
    start_timers();
		alg_Greedy(n, mat1, mat2, sol);
		int costo=costeSolucion(sol,n);
		tiempo = elapsed_time();
		printf("Greedy de %d datos: Coste-> %d. Tiempo-> %fs.\n", n, costo, tiempo);

		printf("Solucion encontrada: ");
		for (int i=0; i<n; i++){
			printf("%d ", sol[i]);
		}
		printf("\n\n");

		//Aplico Busqueda Local
    start_timers();
		alg_Busqueda_Local(n, mat1, mat2, sol, &costo);
    tiempo = elapsed_time();
		printf("Busqueda Local de %d datos: Coste-> %d. Tiempo-> %fs.\n", n, costo, tiempo);
		printf("Solucion encontrada: ");
		for (int i=0; i<n; i++){
			printf("%d ", sol[i]);
		}
		printf("\n\n");
	}

  return 0;
}
