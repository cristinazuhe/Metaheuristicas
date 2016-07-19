#include <stdio.h>
#include <stdlib.h>
#include "graps.h"
#include <stdbool.h>


void Etapa2(int *solucion, int n, int **mat1, int **mat2, int u1, int l1, int u2, int l2){
    //Matriz en la que pondré los costes actuales a partir de las l y u usadas
    int **mataux = (int **)malloc(n*sizeof(int*));
    for (int i=0; i<n; i++){
      mataux[i] = (int*)malloc(n*sizeof(int));
    }
    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++){
            if (i==u1 || i==u2 || j==l1 || j==l2)
                mataux[i][j]=-1;
            else
                mataux[i][j]=0;
        }
    }
    //Obtengo nuevo umbral
    for (int i=0; i<n-2; i++){
        int cmin = 99999, cmax=0;
        for (int j=0; j<n; j++){
            for (int k=0; k<n; k++){
                if (mataux[j][k]!=-1){
                    int costeactual=0;
                    for (int h=0; h<n; h++){
                        if (solucion[h]!=-1){
                            costeactual += mat1[j][h]*mat2[k][solucion[h]];
                        }
                    }
                    mataux[j][k]=costeactual;
                    if (cmin > costeactual) cmin = costeactual;
                    if (cmax < costeactual) cmax = costeactual;
                }
            }
        }
        int nuevoumbral = cmin + (0.3*(cmax - cmin));
        int posibles = 0;
        int vectoru[n*(n-(i+2))];
        int vectorl[n*(n-(i+2))];
        for (int j=0; j<n; j++){
            for (int k=0; k<n; k++){
                if (mataux[j][k]>= nuevoumbral){
                    vectoru[posibles]=j;
                    vectorl[posibles]=k;
                    posibles++;
                }
            }
        }
        int aleato = obtener_aleatorio(0,posibles-1);
        int nuevou = vectoru[aleato];
        int nuevol = vectorl[aleato];
        for (int j=0; j<n; j++){
            for (int k=0; k<n; k++){
                if (j==nuevou)
                    mataux[j][k]=-1;
                if (k==nuevol)
                    mataux[j][k]=-1;
            }
        }
        solucion[nuevou]=nuevol;
    }
}

void Etapa1(int *solucion,int n, int **mat1, int **mat2){
    //Calculo los potenciales y inicializo la lista de candidatos
  /*  printf("Solucion etapa1 arriba: ");
           for (int i=0; i<n; i++){
             printf("%d ", solucion[i]);
   }*/
   //Obtengo potencial de unidades y localizaciones
	int potf[n], potd[n];
	for (int i=0; i<n; i++){
		potf[i]=0;
		potd[i]=0;
		solucion[i]=-1;
		for (int j=0; j<n; j++){
			potf[i] += mat1[i][j] + mat1[j][i];
			potd[i] += mat2[i][j] + mat2[j][i];
		}
	}

  //Obengo umbral de ambos potenciales
	int lrcu[n],lrcl[n];
	int usadoslrcu=0, usadoslrcl=0;
  int umbrallrcu=0, umbrallrcl=0;
  int menorlrcu=999999,mayorlrcu=0,menorlrcl=999999,mayorlrcl=0;
  for (int i=0;i<n;i++){
        if (mayorlrcu < potf[i]) mayorlrcu=potf[i];
        if (menorlrcu > potf[i]) menorlrcu=potf[i];
        if (mayorlrcl < potd[i]) mayorlrcl=potd[i];
        if (menorlrcl > potd[i]) menorlrcl=potd[i];
  }
  umbrallrcu = mayorlrcu - (0.3*(mayorlrcu - menorlrcu));
  umbrallrcl = menorlrcl + (0.3*(mayorlrcl - menorlrcl));

 //Me quedo con las unidades y localizaciones que superen los umbrales
  for (int i=0; i<n; i++){
        if (potf[i] >= umbrallrcu){
            lrcu[usadoslrcu]=i;
            usadoslrcu++;
        }
        if (potd[i] >= umbrallrcl){
            lrcl[usadoslrcl]=i;
            usadoslrcl++;
        }
  }

  //Obengo dos unidades y dos localizaciones de cada lista de candidatos. Las intercambiaré en la solución.
  int u1 = obtener_aleatorio(0,usadoslrcu-1);
  int l1 = obtener_aleatorio(0,usadoslrcl-1);
  int u2,l2;
    if (usadoslrcu > 1 && usadoslrcl>1){
        do {
            u2 = obtener_aleatorio(0,usadoslrcu-1);
        } while (u1 == u2);

        do {
            l2 = obtener_aleatorio(0,usadoslrcl-1);
        } while (l1 == l2);

        solucion[lrcu[u1]]=lrcl[l1];
        solucion[lrcu[u2]]=lrcl[l2];
        u1=lrcu[u1];
        u2=lrcu[u2];
        l1=lrcl[l1];
        l2=lrcl[l2];
    }
    else if(usadoslrcu<2 && usadoslrcl>1){
        //Obtengo u2.
        int siguiente=0;
        if(u1==0)
          siguiente=1;
        for (int i=0;i<n;i++){
            if (potf[siguiente]<=potf[i] && lrcu[u1]!=i)
                siguiente=i;
        }
        u2 = siguiente;
        //Obtengo l2.
        do {
            l2 = obtener_aleatorio(0,usadoslrcl-1);
        } while (l1 == l2);
        //Cambiar esto
        solucion[lrcu[u1]]=lrcl[l1];
        solucion[u2]=lrcl[l2];
        u1=lrcu[u1];
        l1=lrcl[l1];
        l2=lrcl[l2];

    }
    else if(usadoslrcu>1 && usadoslrcl<2){
      //Obtengo u2
      do {
          u2 = obtener_aleatorio(0,usadoslrcu-1);
      } while (u1 == u2);
      //Obtengo l2
      int siguiente=0;
      if(l1==0)
        siguiente=1;
      for (int i=0; i<n ; i++){
          if (potd[siguiente]<=potd[i] && lrcl[l1]!=i)
              siguiente=i;
      }
      l2=siguiente;
       //Cambiar esto
              solucion[lrcu[u1]]=lrcl[l1];
              solucion[lrcu[u2]]=l2;
              u1=lrcu[u1];
              u2=lrcu[u2];
              l1=lrcl[l1];
    }else{

      //Obtengo u2.
      int siguiente=0;
      if(u1==0)
        siguiente=1;
      for (int i=0;i<n;i++){
          if (potf[siguiente]<=potf[i] && lrcu[u1]!=i)
              siguiente=i;
      }
      u2 = siguiente;
       //Obtengo l2
      int siguiente2=0;
      if(l1==0)
        siguiente2=1;
      for (int i=0; i<n ; i++){
          if (potd[siguiente2]<=potd[i] && lrcl[l1]!=i)
              siguiente2=i;
      }
      l2=siguiente2;
      //Cambiar esto
      solucion[lrcu[u1]]=lrcl[l1];
      solucion[u2]=l2;
      u1=lrcu[u1];
      l1=lrcl[l1];
   }/*
   printf("\nu1: %d. l1: %d. u2: %d. l2: %d", u1, l1, u2, l2);
    //ETAPA 2
    printf("\nabajo etapa1: ");
                for (int i=0; i<n; i++){
                  printf("%d ", solucion[i]);
            }*/
    Etapa2(solucion,n,mat1,mat2,u1,l1,u2,l2);
}


void alg_Graps(int n, int **mat1, int **mat2, int *solucion, int *costo){
    int contador = 0;
    int *solucion_aux = (int *)malloc(n*sizeof(int));
    for (int i=0; i<n; i++){
        	solucion_aux[i]=solucion[i];
    }
    int coste = costeSolucion(solucion,n);
    int mejorcosto = coste;

    Set_random(550055);
    while (contador < 25){
        //Llamo a la función que me generará la solución inicial
        Etapa1(solucion_aux,n, mat1, mat2);
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
