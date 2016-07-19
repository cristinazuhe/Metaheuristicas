#include <stdio.h>
#include <stdlib.h>
#include "busqueda_local.h"
#include "random.h"

int **cop_mat1, **cop_mat2, copia_n;

//Calculo el coste de cambiar en solucion las unidades r y s
int coste_Posicion(int * solucion, int r, int s){
	int suma = 0;
	for (int k=0; k<copia_n; k++)
		if (k != r && k != s)
			suma += (cop_mat1[r][k]*((cop_mat2[solucion[s]][solucion[k]])-(cop_mat2[solucion[r]][solucion[k]]))) +
			      	(cop_mat1[s][k]*((cop_mat2[solucion[r]][solucion[k]])-(cop_mat2[solucion[s]][solucion[k]]))) +
			      	(cop_mat1[k][r]*((cop_mat2[solucion[k]][solucion[s]])-(cop_mat2[solucion[k]][solucion[r]]))) +
			       	(cop_mat1[k][s]*((cop_mat2[solucion[k]][solucion[r]])-(cop_mat2[solucion[k]][solucion[s]])));
	return suma;
}

//Comprobar si un movimiento mejora y actualizo
bool CheckMove( int * solucion, int i, int j, int *coste){
    bool mejora=false;
    int costecambio = coste_Posicion(solucion,i,j);
    if (costecambio < 0){
        (*coste) = (*coste) + costecambio;
		    int aux = solucion[i];
		    solucion[i]=solucion[j];
		    solucion[j]=aux;
        mejora = true;
    }
    return mejora;
}

//Funcion don´t looks bit
bool dontlooksbit(int * solucion, int *coste, int * dlb){
    bool improve_flag=false;
    for (int i = 0; i<copia_n && !improve_flag; i++){
        if (dlb[i]==0){
            improve_flag=false;
            for (int j=0; j<copia_n; j++){
                if (i != j){
                    if (CheckMove(solucion,i,j,coste)){
                        dlb[i]=0;
                        dlb[j]=0;
                        improve_flag = true;
                    }
                }
            }
            if (!improve_flag){
                dlb[i] = 1;
            }
        }
    }
    return improve_flag;
}

//Algoritmo de busqueda local
void alg_Busqueda_Local(int n, int **mat1, int **mat2, int *solucion, int *costo){
	//Copias auxiliares de matrices y n.
	cop_mat1 = mat1;
	cop_mat2 = mat2;
	copia_n = n;
	Set_random(550055);      //Vamos cambiando la semilla y obtenemos distintos aleatorios
	permutar_Solucion_n(solucion,n); 	//Genero la solución incial
	int coste = costeSolucion(solucion,n);  //Obtengo el coste de la solucion actual

	//creo lista de cantidatos: inicialmente a 0
	int dlb[n];
	for (int i=0; i<n; i++){
		dlb[i]=0;
	}

	bool mejora = true;
	do {
		mejora = dontlooksbit(solucion,&coste,dlb);
	} while (mejora);
	(*costo) = coste;
}
