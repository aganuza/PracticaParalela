/*
 funciones

En este fichero se controlara el movmiento e interación de las personas
*/

#include "estructuras.h"
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


void mover(struct persona *personas, struct poblacion *poblacion, int tam){
/*movimiento*/
    int p;
    int cambiox, cambioy;
    for(p=0; p<tam; p++){
/*si no supera los limites 0 y max le suma el movimiento*/
        if(!personas[p].posicion[0] + personas[p].velocidad[0]> poblacion->limite && !personas[p].posicion[0] + personas[p].velocidad[0]<0){
            personas[p].posicion[0] += personas[p].velocidad[0];
        }else{
/*si supera los limites del espacio, invierte el movimiento y lo aplica*/
            personas[p].velocidad[0] = -personas[p].velocidad[0];
            personas[p].posicion[0] += personas[p].velocidad[0];
        }
        if(!personas[p].posicion[1] + personas[p].velocidad[1]> poblacion->limite && !personas[p].posicion[1] + personas[p].velocidad[1]<0){
            personas[p].posicion[1] += personas[p].velocidad[1];
        }else{
            personas[p].velocidad[1] = -personas[p].velocidad[1];
            personas[p].posicion[1] += personas[p].velocidad[1];
        }
        /*probabilidad de cambio de velocidad*/

        cambiox = (rand()%((poblacion->cambiovel)+1))-(poblacion->cambiovel/2);
        cambioy = (rand()%((poblacion->cambiovel)+1))-(poblacion->cambiovel/2);

        if((!personas[p].velocidad[0] + cambiox) > poblacion->vel_max && !(personas[p].velocidad[1] + cambiox) < -poblacion->vel_max){
        	personas[p].velocidad[0] += cambiox;
        }
		if((!personas[p].velocidad[1] + cambioy) > poblacion->vel_max && !(personas[p].velocidad[1] + cambioy) < -poblacion->vel_max){
        	personas[p].velocidad[1] += cambioy;
        }
    }

}

    //crear tipo de dato persona
void Crear_Tipo (int *A, int *B, int *C, int *D, int *E, int *F, MPI_Datatype *PARAM){
    int tam [6];
    MPI_Aint dist [6], dir1 , dir2; // tipo de las dir. en MPI
    MPI_Datatype tipo [6];
    tam [0] = 1;
    tam [1] = 1;
    tam [2] = 1;
    tam [3] = 1;
    tam [4] = 2;
    tam [5] = 2;
    tipo [0] = MPI_INT;
    tipo [1] = MPI_INT;
    tipo [2] = MPI_INT;
    tipo [3] = MPI_INT;
    tipo [4] = MPI_INT;
    tipo [5] = MPI_INT;

    dist [0] = 0;
    MPI_Get_address (A, &dir1);
    MPI_Get_address (B, &dir2);
    dist [1] = dir2 - dir1;
    MPI_Get_address (C, &dir2);
    dist [2] = dir2 - dir1;
    MPI_Get_address (D, &dir2);
    dist [3] = dir2 - dir1;
    MPI_Get_address (E, &dir2);
    dist [4] = dir2 - dir1;
    MPI_Get_address (F, &dir2);
    dist [5] = dir2 - dir1;

    MPI_Type_create_struct (6, tam , dist , tipo , PARAM);
    MPI_Type_commit (PARAM);
}

int porcentaje(int tam, int n){
    return (n*100/tam);
}

//Escribir resultados en el fichero
void simulacion(struct poblacion *poblacion, struct persona *personas, double t){
    FILE *f1, *f2;
    int sanos=0, infectados=0, sintomas=0, recuperados=0, vacunados=0, muertos=0, i;
    f1=fopen("posiciones.txt","w+");
    f2=fopen("resultados.txt","w+");

    if (f1==NULL || f2==NULL){
        printf("ERROR! No se pudo abrir el documento");
        exit(-1);
    }

    for(i=0; i<poblacion->per_max; i++){
        fprintf(f1,"Persona: %d %d %d\n", i, personas[i].posicion[0], personas[i].posicion[1]);
    }

    for(i=0; i<poblacion->per_max; i++){
        switch (personas[i].estado){
            case 0:
                sanos++;
                break;
            case 1:
                infectados++;
                break;
            case 2:
                sintomas++;
                break;
            case 3:
                recuperados++;
                break;
            case 4:
                vacunados++;
                break;
            case 5:
                muertos++;
                break;
        }
    }

    sanos = porcentaje(poblacion->per_max, sanos);
    infectados = porcentaje(poblacion->per_max, infectados);
    sintomas = porcentaje(poblacion->per_max, sintomas);
    recuperados = porcentaje(poblacion->per_max, recuperados);
    vacunados = porcentaje(poblacion->per_max, vacunados);
    muertos = porcentaje(poblacion->per_max, muertos);

    fprintf(f2,"Simulacion: Sanos: %d%% Infectados: %d%% Sintomas: %d%% Recuperados: %d%% Vacunados: %d%% Muertos: %d%% Tiempo de Ejecución: %f s\n", sanos, infectados, sintomas, recuperados, vacunados, muertos, t);

    fclose(f1);
    fclose(f2);
}
