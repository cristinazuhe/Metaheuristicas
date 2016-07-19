#include <stdio.h>
#include <stdlib.h>
#include "agg.h"
#include <stdbool.h>
int copia_M, copia_n;

void ordenar_Poblacion(int poblacion[copia_M][copia_n+1]){
  //Ordeno la poblacion de soluciones de menor a mayor coste
  for (int i=0; i<copia_M; i++){
      int menorcoste=999999, posmenorcoste=i;
      for (int j=i; j<copia_M; j++){
          if (menorcoste > poblacion[j][copia_n]){
              menorcoste = poblacion[j][copia_n];
              posmenorcoste=j;
          }
      }
      for (int j=0; j<copia_n+1; j++){
          int aux = poblacion[posmenorcoste][j];
          poblacion[posmenorcoste][j] = poblacion[i][j];
          poblacion[i][j] = aux;
      }
  }
}

void seleccion_padres(int poblacion_actual[copia_M][copia_n+1], int  poblacion_padres[copia_M][copia_n+1], int n_padres){
  //Realizando torneo binario me quedo con los n_padres mejores
  for (int i=0; i<n_padres; i++){
      int aux1 = obtener_aleatorio(0,copia_M-1);
      int aux2 = obtener_aleatorio(0,copia_M-1);
      while(aux1 == aux2) {
          aux2 = obtener_aleatorio(0,copia_M-1);
      }
      if (aux1 < aux2){  //la población actual ya está ordenada de menor a mayor coste.
          for (int j=0; j<copia_n+1; j++)
              poblacion_padres[i][j]=poblacion_actual[aux1][j];
      }
      else {
          for (int j=0; j<copia_n+1; j++)
              poblacion_padres[i][j]=poblacion_actual[aux2][j];
      }
  }
}

void mutar_poblacion(int poblacion[copia_M][copia_n+1], int nummutaciones, int n_padres){
  for (int i=0; i<nummutaciones; i++){
      int alea1,alea2,alea3;
      alea1 = obtener_aleatorio(0,n_padres-1);  //Obtengo una solucion particular de la poblacion intermedia
      alea2 = obtener_aleatorio(0,copia_n-1);
      do {
          alea3 = obtener_aleatorio(0,copia_n-1);
      } while (alea2==alea3);
      //Intercambio los elementos alea2 y alea3 de la solucion alea1 de la poblacion intermedia.
      int aux = poblacion[alea1][alea2];
      poblacion[alea1][alea2]= poblacion[alea1][alea3];
      poblacion[alea1][alea3]= aux;
      int solupar[copia_n];
      for (int j=0; j<copia_n; j++){
          solupar[j]=poblacion[alea1][j];
      }
      int cost= costeSolucion(solupar,copia_n);  //Obtengo el coste de la solucion actual
      poblacion[alea1][copia_n]=cost;
  }
}

void alg_Memetico(int n, int **mat1, int **mat2, int *solucion, int *costo, int tipo_operador, int opc_memetico){
  int mejorcoste;
  int M=50;
  copia_M=M;
  copia_n=n;
  //nuevo contador
  int contgeneracion=0;
  Set_random(550055);      //Vamos cambiando la semilla y obtenemos distintos aleatorios
  //Genero matriz (poblacion actual) con M soluciones aleatorias junto con sus costes.
  int poblacionactual[M][n+1];
  for (int i=0; i<M; i++){
    	permutar_Solucion_n(solucion,n); 	//Genero la solución incial
      int costo = costeSolucion(solucion,n);  //Obtengo el coste de la solucion actual
      for (int j=0; j<n; j++){
          poblacionactual[i][j]=solucion[j];
      }
      poblacionactual[i][n]=costo;
  }

  //Ordeno la población actual de menor a mayor coste.
  ordenar_Poblacion(poblacionactual);

  //Obtengo la mejor solución del a población.
  int mejorpadre[n+1];
  for (int i=0; i<n+1; i++){
      mejorpadre[i]= poblacionactual[0][i];
  }

  int n_evaluaciones=0;
  while (n_evaluaciones < 25000){ //BUCLE PRINCIPAL....
      //SELECCION. Creo poblacion de padres:obtengo dos aleatorios y me quedo con el mejor de la poblacion actual
      int poblacionpadres[M][n+1];
      seleccion_padres(poblacionactual, poblacionpadres, M);
      //Ordeno la poblacion de padres de menor a mayor coste
      ordenar_Poblacion(poblacionpadres);

      //CRUCE
      int poblacionintermedia[M][n+1];
      int numcruces = 0.7 * (M/2);
      int indice_auxiliar=0;
      for (int i=0; i<M; i++){
          if (i<numcruces){
            if(tipo_operador==0){ //Hago cruce posicion
              int solcomunes[n];
              int solsueltas[n];
              int numsuletas=0;
              int poslibres[n];
              for(int i=0; i < n; i++){
                solcomunes[i]=-1;
                solsueltas[i]=-1;
                poslibres[i]=-1;
              }

              //Me quedo con los elementos que coinciden en los dos padres y los almaceno en su orden en solcomunes. Tendre dos hijos.
              //Me quedo con los elementos que no coinciden y los coloco en solsueltas, almacenando sus lugares en poslibres
              for (int j=0; j<n; j++){
                  if (poblacionpadres[i][j]==poblacionpadres[i+1][j]){
                      poblacionintermedia[i][j]=poblacionpadres[i][j];
                      poblacionintermedia[i+1][j]=poblacionpadres[i][j];
                      solcomunes[j]=poblacionpadres[i][j];
                  }
                  else {
                      solsueltas[numsuletas]=poblacionpadres[i][j];
                      poslibres[numsuletas]=j;
                      numsuletas++;
                  }

              }
              //COMPLETO PRIMER HIJO...
              //Genero aleatorio con las unidades de solsueltas para completar primer hijo.
              permutar_Solucion_n(solsueltas,numsuletas);

              //Aniado la permutación realizada en orden al primer hijo.
              for (int j=0; j<numsuletas; j++){
                  poblacionintermedia[i][poslibres[j]]=solsueltas[j];
                  solcomunes[poslibres[j]]=solsueltas[j];
              }
              //Obtengo el coste del primer hijo y se lo aniado
              int cos1 = costeSolucion(solcomunes,n);  //Obtengo el coste de la solucion actual
              poblacionintermedia[i][n]=cos1;

              //COMPLETO SEGUNDO HIJO...
              //Genero aleatorio con las unidades de solsueltas para completar segundo hijo.
            	permutar_Solucion_n(solsueltas,numsuletas);

              //Aniado la permutación realizada en orden al segundo hijo.
              for (int j=0; j<numsuletas; j++){
                  poblacionintermedia[i+1][poslibres[j]]=solsueltas[j];
                  solcomunes[poslibres[j]]=solsueltas[j];
              }

              //Obtengo el coste del primer hijo y se lo aniado
              int cos2 = costeSolucion(solcomunes,n);  //Obtengo el coste de la solucion actual
              poblacionintermedia[i+1][n]=cos2;


              n_evaluaciones += 2;
              i++;
              indice_auxiliar++;
            }
            else if(tipo_operador==1){  //Hago cruce OX
                  int auxi1=obtener_aleatorio(0,n-1);
                  int auxi2=obtener_aleatorio(0,n-1);
                  while(auxi1 == auxi2){
                        auxi2=obtener_aleatorio(0,n-1);
                  }
                  if (auxi1 > auxi2){  //Busco que auxi1 sea menor que el auxi2
                      int aux = auxi1;
                      auxi1 = auxi2;
                      auxi2 = aux;
                  }

                  //Copio en los hijos los intervalos fijos de los padres
                  int n_aniadidos=(auxi2-auxi1+1);
                  int n_introducir=n-n_aniadidos;
                  int hijo1[n_aniadidos];
                  int hijo2[n_aniadidos];
                  int contador_aux=0;
                  for (int j=auxi1;j<=auxi2;j++){
                      poblacionintermedia[i][j]=poblacionpadres[i][j];
                      hijo1[contador_aux]=poblacionpadres[i][j];
                      poblacionintermedia[i+1][j]=poblacionpadres[i+1][j];
                      hijo2[contador_aux]=poblacionpadres[i+1][j];
                      contador_aux++;
                  }

                  //Preparamos las localizaciones restantes para ambos hijos en el orden del otro padre
                  int parte2hijo1[n_introducir], parte2hijo2[n_introducir];
                  int cont1=0, cont2=0;
                      for (int j=0;j<n;j++){
                          int metidohijo1=0,metidohijo2=0;
                          for (int k=0;k<n_aniadidos;k++){
                              if (metidohijo1==0){
                                  if (poblacionpadres[i+1][j]==hijo1[k])
                                      metidohijo1=1;
                              }
                              if (metidohijo2==0){
                                  if (poblacionpadres[i][j]==hijo2[k])
                                      metidohijo2=1;
                              }
                          }
                          if (metidohijo1==0){
                             parte2hijo1[cont1]=poblacionpadres[i+1][j];
                             cont1++;
                          }
                          if (metidohijo2==0){
                             parte2hijo2[cont2]=poblacionpadres[i][j];
                              cont2++;
                          }
                    }

                  //Copio en los hijos las localizaciones restantes en el orden del otro padre
                  int nuevo_indice=(auxi2+1)%n;
                  for (int j=0; j<n_introducir; j++){
                          poblacionintermedia[i][nuevo_indice]=parte2hijo1[j];
                          poblacionintermedia[i+1][nuevo_indice]=parte2hijo2[j];
                          nuevo_indice = (nuevo_indice+1)%n;
                  }
                  int hij1[n],hij2[n];
                  for (int j=0; j<n; j++){
                         hij1[j]=poblacionintermedia[i][j];
                         hij2[j]=poblacionintermedia[i+1][j];
                  }

                  //Almaceno el coste de los nuevos hijos
                  poblacionintermedia[i][n] = costeSolucion(hij1,n);
                  poblacionintermedia[i+1][n] = costeSolucion(hij2,n);

                  n_evaluaciones += 2;
                  i++;


            }
          }
          else { //Si no cruzo, aniado los padres que he usado y las siguientes soluciones de mejor calidad
              for (int j=0; j<n+1; j++){
                  poblacionintermedia[i][j]= poblacionpadres[i-2*indice_auxiliar][j];
              }
          }
      }


      //MUTACION
      int nummutaciones = 0.001 * (M*n);
      mutar_poblacion(poblacionintermedia, nummutaciones, M);
      n_evaluaciones = n_evaluaciones+nummutaciones;


//MEMETICO
 contgeneracion++;
      if(contgeneracion%10 == 0){
        //Opcion 0: Aplico bUsqueda local sobre toda la poblacion
        if (opc_memetico==0){
          for (int i=0;i<M;i++){
              int solucionesaux[n];
              int coste_solucionesaux;
              for (int j=0;j<n;j++){
                  solucionesaux[j]=poblacionintermedia[i][j];
              }
              coste_solucionesaux=poblacionintermedia[i][n];
              alg_Busqueda_Local(n,mat1,mat2,solucionesaux,&coste_solucionesaux,0);
              //Si el coste de la nueva solucion es mejor, se reemplaza en la poblacionintermedia
              if (coste_solucionesaux < poblacionintermedia[i][n]){
                  for (int j=0;j<n;j++){
                      poblacionintermedia[i][j]=solucionesaux[j];
                  }
                  poblacionpadres[i][n]=coste_solucionesaux;
              }
              n_evaluaciones++;
          }
        }
        if (opc_memetico==1){
            int por_pob = 0.1*copia_M;
            for (int i=0;i<por_pob;i++){
                int solucionesaux[n];
                int coste_solucionesaux;
                int k = obtener_aleatorio(0,copia_M-1);
                for (int j=0;j<n;j++){
                    solucionesaux[j]=poblacionintermedia[k][j];
                }
                coste_solucionesaux=poblacionintermedia[k][n];
                alg_Busqueda_Local(n,mat1,mat2,solucionesaux,&coste_solucionesaux,0);
                //Si el coste de la nueva solucion es mejor, se reemplaza en la poblacionintermedia
                if (coste_solucionesaux < poblacionintermedia[k][n]){
                    for (int j=0;j<n;j++){
                        poblacionintermedia[k][j]=solucionesaux[j];
                    }
                    poblacionpadres[k][n]=coste_solucionesaux;
                }
                n_evaluaciones++;
            }
        }
        //Busqueda local sobre el 10% de los mejores.
        //Tengo la poblacion ordenada por coste --> 10% primero
        if (opc_memetico==2){
            int por_pob = 0.1*copia_M;
            for (int i=0;i<por_pob;i++){
                int solucionesaux[n];
                int coste_solucionesaux;
                for (int j=0;j<n;j++){
                    solucionesaux[j]=poblacionintermedia[i][j];
                }
                coste_solucionesaux=poblacionintermedia[i][n];
                alg_Busqueda_Local(n,mat1,mat2,solucionesaux,&coste_solucionesaux,0);
                //Si el coste de la nueva solucion es mejor, se reemplaza en la poblacionintermedia
                if (coste_solucionesaux < poblacionintermedia[i][n]){
                    for (int j=0;j<n;j++){
                        poblacionintermedia[i][j]=solucionesaux[j];
                    }
                    poblacionpadres[i][n]=coste_solucionesaux;
                }
                n_evaluaciones++;
            }
        }
      }





      //REEMPLAZAMIENTO
      //Busco el peor de la poblacion intermedia y lo sustituyo por el mejor padre que teniamos
      int pospeor=0,costpeor=0;
      for (int i=0; i<M; i++){
          if (costpeor < poblacionintermedia[i][n]){
              pospeor = i;
              costpeor = poblacionintermedia[i][n];
          }
      }
      for (int i=0; i<n+1; i++){
          poblacionintermedia[pospeor][i]=mejorpadre[i];
      }

      //Ordeno la poblacion intermedia
      ordenar_Poblacion(poblacionintermedia);

      //La poblacion actual sera la poblacion intermecia
      for (int i=0; i<M; i++){
          for(int j=0; j<n+1; j++){
              poblacionactual[i][j]=poblacionintermedia[i][j];
          }
      }

      //Guardo el mejor padre de la poblacion actual
      for (int i=0; i<n+1; i++){
          mejorpadre[i]=poblacionactual[0][i];
          if (i<n)
              solucion[i]=poblacionactual[0][i];
      }
      mejorcoste = poblacionactual[0][n];
  }
  (*costo) = mejorcoste;
}
