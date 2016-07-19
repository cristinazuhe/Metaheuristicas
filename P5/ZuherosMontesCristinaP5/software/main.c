#include <stdio.h>
#include <stdlib.h>
#include "lectura.h"
#include "greedy.h"
#include "timer.h"
#include "busqueda_local.h"
#include "agg.h"
int **mat1, **mat2, *sol;

void reservarEspacio(int n){
	free(sol);
	free(mat1);
	free(mat2);
	sol=NULL;
	mat1=NULL;
	mat2=NULL;
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

//Aplicamos todos los algoritmos a todos los archivos
void TodosArchivos(){
    int n, costo=0;
    int nficheros=20;
    char *nombres[nficheros];
    double tiempo;
    DarNombres(nombres);

    int finales[nficheros][5];
    float tiempos[nficheros][5];
		int tamanio[nficheros];
		int resultadosoptimos[nficheros];
		resultadosoptimos[0]=2192;
		resultadosoptimos[1]=14142;
		resultadosoptimos[2]=6194;
		resultadosoptimos[3]=3796;
		resultadosoptimos[4]=130;
		resultadosoptimos[5]=116;
		resultadosoptimos[6]=64;
		resultadosoptimos[7]=88700;
		resultadosoptimos[8]=360630;
		resultadosoptimos[9]=15812;
		resultadosoptimos[10]=23386;
		resultadosoptimos[11]=90998;
		resultadosoptimos[12]=115534;
		resultadosoptimos[13]=140691;
		resultadosoptimos[14]=1855928;
		resultadosoptimos[15]=13499184;
		resultadosoptimos[16]=15844731;
		resultadosoptimos[17]=441786736;
		resultadosoptimos[18]=43849646;
		resultadosoptimos[19]=7620628;
    double desviacion_total[5];
		double desviaciones[nficheros][5];

    for (int i=0; i<nficheros; i++){
        n = leerNumeroDatos(nombres[i]);
        reservarEspacio(n);
        obtenerMatrices(nombres[i], mat1, mat2);
        tamanio[i]=n;

			  //Aplico Greedy
        start_timers();
        alg_Greedy(n, mat1, mat2, sol, &costo);
        tiempo = elapsed_time();
        finales[i][0]=costo;
        tiempos[i][0]=tiempo;
        desviaciones[i][0]=(finales[i][0] - resultadosoptimos[i])*10;
        desviaciones[i][0] = desviaciones[i][0]/resultadosoptimos[i];
        desviaciones[i][0] = desviaciones[i][0]*10;

			  //Aplico AGG posicional
        start_timers();
        alg_Memetico(n, mat1, mat2, sol, &costo,0,0);
        tiempo = elapsed_time();
        finales[i][1]=costo;
        tiempos[i][1]=tiempo;
        desviaciones[i][1]=(finales[i][1] - resultadosoptimos[i])*10;
        desviaciones[i][1] = desviaciones[i][1]/resultadosoptimos[i];
        desviaciones[i][1] = desviaciones[i][1]*10;

		    alg_Greedy(n, mat1, mat2, sol, &costo);
				//Aplico AGG posicional
				start_timers();
				alg_Memetico(n, mat1, mat2, sol, &costo,0,1);
				tiempo = elapsed_time();
				finales[i][2]=costo;
				tiempos[i][2]=tiempo;
				desviaciones[i][2]=(finales[i][2] - resultadosoptimos[i])*10;
				desviaciones[i][2] = desviaciones[i][2]/resultadosoptimos[i];
				desviaciones[i][2] = desviaciones[i][2]*10;

	    	alg_Greedy(n, mat1, mat2, sol, &costo);
				//Aplico AGG posicional
				start_timers();
				alg_Memetico(n, mat1, mat2, sol, &costo,0,2);
				tiempo = elapsed_time();
				finales[i][3]=costo;
				tiempos[i][3]=tiempo;
				desviaciones[i][3]=(finales[i][3] - resultadosoptimos[i])*10;
				desviaciones[i][3] = desviaciones[i][3]/resultadosoptimos[i];
				desviaciones[i][3] = desviaciones[i][3]*10;

  }

   int cont1=0,cont2=0,cont3=0,cont4=0, cont5=0;
    for (int i=0; i<nficheros; i++){
        int mejor=0;
        if (finales[i][mejor]>=finales[i][1])
            mejor=1;
		    if (finales[i][mejor]>=finales[i][2])
		        mejor=2;
		    if (finales[i][mejor]>=finales[i][3])
		        mejor=3;
        if (mejor == 0) cont1++;
        if (mejor == 1) cont2++;
        if (mejor == 2) cont3++;
        if (mejor == 3) cont4++;

        printf("Problema: %s con n: %d\n",nombres[i],tamanio[i]);
        printf("\tGreeedy: %d en %f\n", finales[i][0],tiempos[i][0]);
        printf("\tAM0: %d en %f\n", finales[i][1],tiempos[i][1]);
        printf("\tAM1: %d en %f\n", finales[i][2],tiempos[i][2]);
        printf("\tAM2: %d en %f\n", finales[i][3],tiempos[i][3]);
    }
    printf("\nNuevo de veces que es mejor:\n Greedy: %d\n AM0: %d\n AM1: %d\n AM2: %d\n",
		       cont1, cont2, cont3, cont4);
    for(int j=0; j< nficheros; j++){
	desviacion_total[0] += desviaciones[j][0];
	desviacion_total[1] += desviaciones[j][1];
	desviacion_total[2] += desviaciones[j][2];
	desviacion_total[3] += desviaciones[j][3];
   }
   desviacion_total[0] = desviacion_total[0]/nficheros;
   desviacion_total[1] = desviacion_total[1]/nficheros;
   desviacion_total[2] = desviacion_total[2]/nficheros;
   desviacion_total[3] = desviacion_total[3]/nficheros;

   printf("\nDesviaciones: \n Greedy: %f\n AM0: %f\n AM1: %f\n AM2: %f\n",
		desviacion_total[0], desviacion_total[1], desviacion_total[2], desviacion_total[3]);
}

int main(int argc, char * argv[])
{
    int todos = 1;
    if (todos == 1){
        TodosArchivos();
        return 0;
    }

	int n = leerNumeroDatos(argv[1]);
	if (n > 0) {
		reservarEspacio(n);
		obtenerMatrices(argv[1], mat1, mat2);
    double tiempo;
    int costo;
		//Aplico Greedy
    start_timers();
		alg_Greedy(n, mat1, mat2, sol, &costo);
		tiempo = elapsed_time();
		printf("Greedy de %d datos: Coste-> %d. Tiempo-> %fs.\n", n, costo, tiempo);

		printf("Solucion encontrada: ");
		for (int i=0; i<n; i++){
			printf("%d ", sol[i]);
		}
		printf("\n\n");

		//Aplico AM0
    start_timers();
		alg_Memetico(n, mat1, mat2, sol, &costo, 0,0); 
    tiempo = elapsed_time();
		printf("Algoritmos Memeticos 0 de %d datos: Coste-> %d. Tiempo-> %fs.\n", n, costo, tiempo);
		printf("Solucion encontrada: ");
		for (int i=0; i<n; i++){
			printf("%d ", sol[i]);
		}
		printf("\n\n");

		alg_Greedy(n, mat1, mat2, sol, &costo);
		//Aplico AM1
		start_timers();
		alg_Memetico(n, mat1, mat2, sol, &costo, 0,1); 
		tiempo = elapsed_time();
		printf("Algoritmos Memeticos 1 de %d datos: Coste-> %d. Tiempo-> %fs.\n", n, costo, tiempo);
		printf("Solucion encontrada: ");
		for (int i=0; i<n; i++){
			printf("%d ", sol[i]);
		}
		printf("\n\n");


		alg_Greedy(n, mat1, mat2, sol, &costo);
		//Aplico AM2
		start_timers();
		alg_Memetico(n, mat1, mat2, sol, &costo, 0,2); 
		tiempo = elapsed_time();
		printf("Algoritmos Memeticos 2 de %d datos: Coste-> %d. Tiempo-> %fs.\n", n, costo, tiempo);
		printf("Solucion encontrada: ");
		for (int i=0; i<n; i++){
			printf("%d ", sol[i]);
		}
		printf("\n\n");


	}

  return 0;
}
