#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "busqueda_tabu.h"
#include "random.h"
#include <stdbool.h>
void alg_Busqueda_Tabu(int n, int **mat1, int **mat2, int *solucion, int *costo){
  //Generar solución inicial: aleatoria
	int *solucion_aux = (int *)malloc(n*sizeof(int));
	Set_random(550055);      //Vamos cambiando la semilla y obtenemos distintos aleatorios
	permutar_Solucion_n(solucion,n);
	(*costo) = costeSolucion(solucion,n);
	for (int i=0; i<n; i++){
		solucion_aux[i]=solucion[i];
	}

	//Inicializamos componentes de BT
	int costepri;
	int tamtabu=n/2;
	int listatabu[n][4];
	int listaveci[30][5];
	int cont = 0;

	while (cont < 25000){
		int mejor=0;
		int r,s;
		int pos_tabu=-1;
    int usadostabu=0;
		//Obtengo matriz con los 30 vecinos
		for(int i=0; i<30; i++){
		    	permutar_Solucion(solucion_aux,&r,&s);
		    	int difcoste = -coste_Posicion(solucion_aux,r,s);
					bool repetido=false;
					for(int k=0; k < i; k++){  //Busco si el vecino generado ya existe.
			   		if(r == listaveci[k][2] && s == listaveci[k][3]){
					   	i--;
					  	repetido=true;
				  	}
			  	}

					if(!repetido){  //Si ese vecino no existe aún, lo añado en orden
		    	    int h=0;
              bool mejora = false;
		    	    for (int j=0; j<i && !mejora; j++){
		    	    	if(difcoste < listaveci[j][4]){ //Veo si el nuevo vecino tiene coste mejor que alguno ya existente
		    	    		h=j;
		    	    		mejora=true;
		    	    	}
		    	    }
					    if(!mejora){ //Si el nuevo vecino no da mejor diferencia de coste, lo introduzco al final de la matriz
                	listaveci[i][0]=solucion[r];
	                listaveci[i][1]=solucion[s];
                	listaveci[i][2]=r;
	                listaveci[i][3]=s;
	                listaveci[i][4]=difcoste;
					    }else{      //Si da mejor diferencia de coste que algún elemento de la matriz, lo coloco en su posición
		            	for (int k=i; k>h; k--){  //Desplazo todos los peor coste
		    	          	listaveci[k][0]=listaveci[k-1][0];
		              	 	listaveci[k][1]=listaveci[k-1][1];
		    	          	listaveci[k][2]=listaveci[k-1][2];
		    	          	listaveci[k][3]=listaveci[k-1][3];
		              		listaveci[k][4]=listaveci[k-1][4];
		           	}//Introduzco el nuevo vecino en su posición para que los costes queden ordenados
		          	listaveci[h][0]=solucion[r];
		           	listaveci[h][1]=solucion[s];
		           	listaveci[h][2]=r;
		          	listaveci[h][3]=s;
		          	listaveci[h][4]=difcoste;
			      	}
						}

			    //Vuelvo a la solución de partida para generar otro vecino.
		      int aux1 = solucion_aux[r];
          solucion_aux[r] = solucion_aux[s];
          solucion_aux[s] = aux1;

		}

	 //Me quedo con el mejor vecino no tabú o que supere el criterio de aspiracion
    bool estabu=false;
  	bool paro_buscar_tabu=false;
		while (!paro_buscar_tabu){
			estabu=false;
			for (int i=0; i<usadostabu; i++){
			  	if (((listaveci[mejor][0] == listatabu[i][0] && listaveci[mejor][2] == listatabu[i][2]) ||
						   (listaveci[mejor][1]==listatabu[i][1] && listaveci[mejor][3]==listatabu[i][3]) )    ||
						  ((listaveci[mejor][0] == listatabu[i][1] && listaveci[mejor][2] == listatabu[i][3]) ||
		 				  	(listaveci[mejor][1]==listatabu[i][0] && listaveci[mejor][3]==listatabu[i][2]))){
				   		estabu=true;
				  		pos_tabu=mejor;
				  }
      }

			if (!estabu){//es no tabu
				paro_buscar_tabu=true;
			}	else { //tengo que comprobar que supere el criterio de aspiración

				if (listaveci[mejor][4]<0){
					paro_buscar_tabu=true;
				}
				else{
					mejor++;
				}
			}
		}


	//Si no es tabu....
		if (!estabu){
			r=listaveci[mejor][2];
			s=listaveci[mejor][3];
			int aux1 = solucion_aux[r];
      solucion_aux[r] = solucion_aux[s];
      solucion_aux[s] = aux1;
			costepri = costeSolucion(solucion_aux,n);

			if (costepri < (*costo)){
			  	(*costo)=costepri;
			  	aux1 = solucion[r];
          solucion[r] = solucion[s];
        	solucion[s] = aux1;

		    	//Meto la solucion como tabu
		      if (usadostabu==0){
          	listatabu[0][0]=solucion[r];
		    		listatabu[0][1]=solucion[s];
		    		listatabu[0][2]=r;
		    		listatabu[0][3]=s;
		    		usadostabu++;
          }
          else {
		    		if (usadostabu >= tamtabu-1){
		    			for(int i=tamtabu-1; i>0;i--){
		    				listatabu[i][0]=listatabu[i-1][0];
		    				listatabu[i][1]=listatabu[i-1][1];
		    				listatabu[i][2]=listatabu[i-1][2];
		    				listatabu[i][3]=listatabu[i-1][3];
		    			}
		    			listatabu[0][0]=solucion[r];
		    			listatabu[0][1]=solucion[s];
		    			listatabu[0][2]=r;
		    			listatabu[0][3]=s;
		    			if(usadostabu<tamtabu)
		    				usadostabu++;
		    		}
		    		else {
		    			for(int i=usadostabu; i>0;i--){
		    				listatabu[i][0]=listatabu[i-1][0];
		    				listatabu[i][1]=listatabu[i-1][1];
		    				listatabu[i][2]=listatabu[i-1][2];
		    				listatabu[i][3]=listatabu[i-1][3];
		    			}
		    			listatabu[0][0]=solucion[r];
		    			listatabu[0][1]=solucion[s];
		    			listatabu[0][2]=r;
		    			listatabu[0][3]=s;
		    			if(usadostabu<tamtabu)
		    				usadostabu++;
		    		}
		    	}
	    }else{
			aux1 = solucion_aux[r];
	    solucion_aux[r] = solucion_aux[s];
	    solucion_aux[s] = aux1;
      }
		}
		//Si es tabu haré prueba de aspiración
		else {
			if (listaveci[mejor][4]<0){
				r=listaveci[mejor][2];
				s=listaveci[mejor][3];
				int aux1 = solucion_aux[r];
	      solucion_aux[r] = solucion_aux[s];
	      solucion_aux[s] = aux1;
				costepri = costeSolucion(solucion_aux,n);
				if (costepri < (*costo)){
            (*costo)=costepri;
            aux1 = solucion[r];
            solucion[r] = solucion[s];
            solucion[s] = aux1;
		      	//Como lo he usado lo pondré el último
	      		int au1 = listaveci[mejor][1];
		      	int au2 = listaveci[mejor][0];
	 	      	int au3 = listaveci[mejor][2];
		      	int au4 = listaveci[mejor][3];
		      	for (int i=0; i<mejor; i++){
			       	listatabu[i][0]=listatabu[i+1][0];
		      		listatabu[i][1]=listatabu[i+1][1];
		      		listatabu[i][2]=listatabu[i+1][2];
		      		listatabu[i][3]=listatabu[i+1][3];
		      	}
		      	listatabu[0][0]=au1;
	      		listatabu[0][1]=au2;
	      		listatabu[0][2]=au3;
	      		listatabu[0][3]=au4;
		  	}else{
				aux1 = solucion_aux[r];
		    solucion_aux[r] = solucion_aux[s];
		    solucion_aux[s] = aux1;
        }
			}
		}

		cont++;
	}
	(*costo) = costeSolucion(solucion,n);
}
