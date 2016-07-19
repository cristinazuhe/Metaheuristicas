#ifndef _AGG_H_
#define _AGG_H_
int M, n;
void ordenar_Poblacion(int poblacion[M][n]);
void seleccion_padres(int poblacion_actual[M][n], int  poblacion_padres[M][n], int n_padres);
void mutar_poblacion(int poblacion[M][n], int nummutaciones, int n_padres);
void alg_AGGeneracionales(int n, int **mat1, int **mat2, int *solucion, int *costo, int tipo_operador);
void alg_AGEstacionarios(int n, int **mat1, int **mat2, int *solucion, int *costo, int tipo_operador);

#endif
