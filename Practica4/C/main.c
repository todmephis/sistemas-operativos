#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <semaforos.h>
#define FTOK_CHAR "/bin/ls"
#define FTOK_INT 94
#define SLOTSMEMORIA 5
#define PRODUCTOR 0
#define CONSUMIDOR 1
#define N_PRODUC 1000
void usage(const char *);

int main(int argc, char const **argv) {
	if(argc != 3){
		fprintf(stderr, "Error en argumentos.\n");
    usage(argv[0]);
    exit(EXIT_FAILURE);
	}
  if((atoi(argv[1])<0 && atoi(argv[1])>1)){
    fprintf(stderr, "Error en argumentos: MODO debe ser 0 ó 1\n");
    usage(argv[0]);
    exit(EXIT_FAILURE);
  }
  if(((atoi(argv[1]) == 0)  && (atoi(argv[2]) < 32)) || ((atoi(argv[1]) == 0) && (atoi(argv[2]) > 128))){
    fprintf(stderr, "Error en argumentos: SIMBOLO_ASCII debe ser un caracter imprimible\n");
    usage(argv[0]);
    exit(EXIT_FAILURE);
  }
  if( atoi(argv[1]) == 1 && atoi(argv[2]) != -1 ){
    fprintf(stderr, "Error en argumentos: si MODO está activado con 1 entonces SIMBOLO_ASCII debe ser -1\n");
    usage(argv[0]);
    exit(EXIT_FAILURE);    
  }

  key_t key;
  int mem_sharedID, localerror, semID;
  int sem_len = SLOTSMEMORIA + 2;
  int contador = 0;
  int contador_c = 0, b_c = 0, c_c = 0;
  int contador_p = 0, b_p = 0, c_p = 0;
  zonacritica * ZC_local = NULL;
  if ((key=ftok(FTOK_CHAR, FTOK_INT)) == -1) {
    localerror = errno;
    perror("Ups, algo nos salió mal");
    exit(localerror);
  }
  if((mem_sharedID=shmget(key, sizeof(zonacritica)*SLOTSMEMORIA, 0777 | IPC_EXCL)) != -1 && (semID=semget(key, sem_len, IPC_EXCL | 0777)) != -1){//Existen se ligan
    printf("Memoria ya creada, voy a ligarme [%d]\n", mem_sharedID);
    mem_sharedID = shmget(key, sizeof(zonacritica)*SLOTSMEMORIA, 0777);
    semID = semget(key,sem_len, 0777);
    ZC_local = (zonacritica *)shmat(mem_sharedID, 0, 0);
  }
  else if((mem_sharedID=shmget(key, sizeof(zonacritica)*SLOTSMEMORIA, 0777 | IPC_EXCL)) == -1 && (semID=semget(key, sem_len, IPC_EXCL | 0777)) == -1){//No existe, se crean
    mem_sharedID = shmget(key, sizeof(zonacritica)*SLOTSMEMORIA, 0777 | IPC_CREAT);//Creamos la memoria compartida
    semID = semget(key,sem_len,IPC_CREAT | 0777);//Se crea el semaforo
    printf("La memoria compartida no existe, voy a crearla [%d]\n", mem_sharedID);
    ZC_local = (zonacritica *)shmat(mem_sharedID, 0, 0);
    for(int semaforo_n = 0; semaforo_n < sem_len; semaforo_n++){
      if(semaforo_n == PRODUCTOR){
        semctl(semID, semaforo_n, SETVAL, SLOTSMEMORIA);
      }
      else if(semaforo_n == CONSUMIDOR){
        semctl(semID, semaforo_n, SETVAL, 0);
      }
      else
        semctl(semID, semaforo_n, SETVAL, 1);
    }
    printf("Semaforos creados satisfactoriamente\n");
    for(contador=0;contador<SLOTSMEMORIA;contador++){
      (ZC_local+contador)->estado = 1;
    }
  }
  else{
    printf("Algo salio extraño\n");
    exit(EXIT_FAILURE);
  }
  if(atoi(argv[1])==PRODUCTOR){
  while(1){
    for(contador_p = 0; contador_p < SLOTSMEMORIA; contador_p++){     
        if(semctl(semID, contador_p + 2, GETVAL, NULL)==1){//1 significa semaforo verde y puedo escribir
           if(semctl(semID, PRODUCTOR, GETVAL, NULL) > 0){
            if((ZC_local+contador_p)->estado == 1){
              wait1(PRODUCTOR,semID);
              wait1(contador_p+2,semID);//pone el semaforo en rojo
              if((ZC_local+contador_p)->estado == 1){
                for(b_p=0;b_p<9;b_p++){
                  (ZC_local+contador_p)->espacio[b_p] = atoi(argv[2]);
                }
                c_p++;
              }
              (ZC_local+contador_p)->estado = 0;//estado 0 = no puedo escribir
              signal1(CONSUMIDOR,semID);
              signal1(contador_p+2,semID);
              printf("%d\n",c_p);
              if(c_p>=N_PRODUC){
                exit(EXIT_SUCCESS);
              }
              break;
            }
          }
        }
    }
  }
  }
  else if(atoi(argv[1])==CONSUMIDOR){
  while(1){
    for(contador_c = 0; contador_c < SLOTSMEMORIA; contador_c++){
      if(semctl(semID, contador_c + 2, GETVAL, NULL)==1){
        if(semctl(semID, CONSUMIDOR, GETVAL, NULL) > 0){
          if((ZC_local+contador_c)->estado == 0){
            wait1(CONSUMIDOR,semID);
            wait1(contador_c+2,semID);
            if((ZC_local+contador_c)->estado == 0){
              printf("Consumo#[%d]Zona[%d] Estado:%d Datos:%s\n", c_c+1, contador_c+1, (ZC_local+contador_c)->estado, (ZC_local+contador_c)->espacio);
              c_c++;
            }
            for(b_c=0;b_c<9;b_c++){
              (ZC_local+contador_c)->espacio[b_c] = '|';
            }
            (ZC_local+contador_c)->estado = 1;
            signal1(PRODUCTOR,semID);
            signal1(contador_c+2,semID);
            if(c_c>=N_PRODUC)
               exit(EXIT_SUCCESS);
            break;
          }
        }
      }
    }
  }    
  }
  else{
    fprintf(stderr, "Error en argumentos.\n");
    usage(argv[0]);
    exit(EXIT_FAILURE);
  }
  return 0;
}

void usage(const char * nombreprograma){
  printf("Usage: %s MODO SIMBOLO_ASCII\n", nombreprograma);
  printf("MODO:\n0 = PRODUCTOR\n1 = CONSUMIDOR\n\n");
  printf("Si es un productor entonces SIMBOLO_ASCII será un entero imprimible del código ASCII\n");
  printf("P.j: %s 0 97\n", nombreprograma);
  printf("97 en el código ASCII pertenece a la letra 'a'\n");
  printf("\nSi es un consumidor entonces SIMBOLO_ASCII deberá ser el entero -1\n");
  printf("P.j: %s 1 -1\n", nombreprograma);
}


