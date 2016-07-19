#include <stdio.h>
#include <stdlib.h>
#include "lectura.h"

int leerNumeroDatos(char * nombre){
	int n = 0;
	FILE *mi_fichero;
    if (nombre != 0){
    	mi_fichero = fopen(nombre,"r");
	    if (mi_fichero){
		    fscanf(mi_fichero, " %d", &n);
		    fclose(mi_fichero);
		  }
		}
	return n;
}

void obtenerMatrices(char * nombre, int **mat1, int **mat2){
	FILE *fichero;
    if (nombre != 0){
    	fichero = fopen(nombre,"r");
	    if (fichero){
	    	int n=0;
		    fscanf(fichero, " %d", &n);
		    for (int i=0; i<n; i++)
		    	for (int j=0; j<n; j++)
		    		fscanf(fichero, " %d", &mat1[i][j]);
		    for (int i=0; i<n; i++)
		    	for (int j=0; j<n; j++)
		    		fscanf(fichero, " %d", &mat2[i][j]);
		    fclose(fichero);
		}
	}
}
