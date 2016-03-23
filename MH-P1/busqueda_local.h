#ifndef _BUSQUEDA_LOCAL_H_
#define _BUSQUEDA_LOCAL_H_
#include <stdbool.h>

void permutar_Solucion(int *solucion, int veces);

int CostePosicion(int * solucion, int r, int s);

bool CheckMove(int * solucion, int i, int j, int *cost);

bool dontlooksbit(int * solucion, int *coste, int * dlb);

void alg_Busqueda_Local(int n, int **mat1, int **mat2, int *solucion, int *costo);

#endif
