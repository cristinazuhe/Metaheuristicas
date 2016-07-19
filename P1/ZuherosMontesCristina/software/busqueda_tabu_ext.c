#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "busqueda_tabu_ext.h"
#include "random.h"
#include <stdbool.h>
void alg_Busqueda_Tabu_Ext(int n, int **mat1, int **mat2, int *solucion, int *costo){
  //Generar solución inicial aleatoria
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

	int mem_largo_plazo[n][n];
	for (int i=0;i<n;i++){
		for(int k=0;k<n;k++){
			mem_largo_plazo[i][k]=0;
		}
	}
	int reinicializaciones=0;
	int contaveces=0;

int minimo_coste=999999999;
int *solucion_minimo_coste = (int *)malloc(n*sizeof(int));

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


		///Si no es tabu...
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
			contaveces++;
			aux1 = solucion_aux[r];
	    solucion_aux[r] = solucion_aux[s];
	    solucion_aux[s] = aux1;
      }
		}
		//Si es tabu haré la prueba de aspiración
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
			  contaveces++;
				aux1 = solucion_aux[r];
		    solucion_aux[r] = solucion_aux[s];
		    solucion_aux[s] = aux1;
        }
			}
		}

	//Actualizo la memoria de a largo plazo
		for(int i=0;i<n;i++){
				int loc=solucion_aux[i];
				mem_largo_plazo[i][loc] += 1;
		}

		if (contaveces==10 && cont< 24900){
			if((*costo) < minimo_coste){
					minimo_coste = (*costo);
					for (int i=0; i<n; i++){
						solucion_minimo_coste[i]=solucion[i];
					}
			}
	          contaveces=0;
	          reinicializaciones++;
	          int criterioreinicializacion = obtener_aleatorio(1,4);
	           if(criterioreinicializacion==1){ //Me voy a una solución aleatoria
	            	permutar_Solucion_n(solucion,n);
								(*costo) = costeSolucion(solucion,n);
	            	for (int i=0;i<n; i++){
	                	solucion_aux[i]=solucion[i];
                }
	            }
	            else if (criterioreinicializacion==2 || criterioreinicializacion==3){ //Me voy a la mejor solución global
								(*costo) = minimo_coste;
								for (int i=0; i<n; i++){
	             		solucion_aux[i]=solucion_minimo_coste[i];
									solucion[i]=solucion_minimo_coste[i];
								}
	            }
	            else { //Hago la memoria de frecuencias y me muevo a la solución resultante
	            	int minx=0,miny=0;
	            	int unidades[n], localidades[n];
	            	for (int i=0;i<n;i++){
	            		unidades[i]=0;
	            		localidades[i]=0;
	            	}
	            	for (int j=0;j<n;j++){
	            	int	minimo=99999999;
		            	for(int i=0;i<n;i++){
		            		for(int k=0;k<n;k++){
		            			if (mem_largo_plazo[i][k]<minimo){
		            				if (unidades[i]!=0 && localidades[k]!=0){
		            					minimo=mem_largo_plazo[i][k];
		            					minx=i;
		            					miny=k;
		            				}
		            			}
		            		}
		            	}
		            	unidades[minx]=0;
		            	localidades[miny]=0;
		            	solucion_aux[minx]=miny;
		            }
	            }
	           if(reinicializaciones%2==1){
	            	tamtabu = tamtabu*2;
	            }
	            else{
	            	tamtabu = tamtabu/2;
	            	if(usadostabu>tamtabu)
	            		usadostabu=tamtabu;
						}
			}
			cont++;


		}
	(*costo) = costeSolucion(solucion,n);
}
