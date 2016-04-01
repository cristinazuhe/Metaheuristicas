#ifndef _ENFRIAMIENTOSIMULADO_H_
#define _ENFRIAMIENTOSIMULADO_H_

float enfriar_temperatura(float t_inicial, float t_final,  float t_actual, int m);

void permutar_Solucion(int * solucion, int *pos1, int *pos2);

void alg_Enfriamiento_Simulado(int n, int *solucion, int *costo);

#endif
