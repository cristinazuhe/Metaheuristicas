#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "enfriamiento_simulado.h"
#include "random.h"

int copia_n;

//Enfriamos la temperatura.
float enfriar_temperatura(float t_inicial, float t_final, float t_actual, int M){
    float beta = (t_inicial-t_final)/(M*t_inicial*t_final);
    return (t_actual)/(1+(beta*t_actual));
}

//Genero un vecino
void permutar_Solucion(int * solucion, int *pos1, int *pos2){
    int aux;
    *pos1 = obtener_aleatorio(0,copia_n-1);
    *pos2 = obtener_aleatorio(0,copia_n-1);
    while(*pos1==*pos2){
      *pos2 = obtener_aleatorio(0,copia_n-1);
    }
    aux = solucion[*pos1];
    solucion[*pos1] = solucion[*pos2];
    solucion[*pos2] = aux;

}


//Algoritmo de enfriamiento simulado
void alg_Enfriamiento_Simulado(int n, int *solucion, int *mejorcosto){
	//Genero solución incial
  permutar_Solucion_n(solucion,n);
	int *solucion_aux = (int *)malloc(n*sizeof(int));
  solucion_aux=solucion;
	copia_n = n;
  int costeact=costeSolucion(solucion_aux,n);
  (*mejorcosto)=costeact;

  float t_inicial = (0.3*costeact)/(-(log(0.3)));
  float t_final = 0.003;
  float t_actual = t_inicial;

  int k = 1.3806488*pow(10,-23);
  int max_vecinos = 10*n;
  int max_exitos = 0.1*max_vecinos;
  int n_enfriamientos = 25000/max_vecinos;

  //Bucle principal del ES
  for (int i=0; i<n_enfriamientos; i++){
      int n_exitos=0, pos1, pos2;
      for (int k=0; k<max_vecinos; k++){
          permutar_Solucion(solucion_aux,&pos1,&pos2);
          int costepri = costeSolucion(solucion_aux,n);
          int diferenciacoste = costepri - costeact;
          double aux = ((double) -diferenciacoste/(k*t_actual));

          if (diferenciacoste < 0 || (obtener_aleatorio(0,1) <  exp(aux))){
              n_exitos++;
              costeact = costepri;
              if (costeact < (*mejorcosto)){
                  (*mejorcosto) = costeact;
                  solucion=solucion_aux;
              }
          }
          else {
              int aux1 = solucion_aux[pos1];
              solucion_aux[pos1] = solucion_aux[pos2];
              solucion_aux[pos2] = aux1;
          }
          if (n_exitos >= max_exitos) break;
      }
      t_actual = enfriar_temperatura(t_inicial, t_final, t_actual, n_enfriamientos);
      if(n_exitos == 0) break;
  }
}
