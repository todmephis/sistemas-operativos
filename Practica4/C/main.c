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
#define FTOK_INT 4
#define SLOTSMEMORIA 10
#define PRODUCTOR 0
#define CONSUMIDOR 1
int main(int argc, char const **argv) {
	if(argc != 3){
		printf("No args\n");
    exit(EXIT_FAILURE);
	}
  key_t key,cont;
  int mem_sharedID, localerror, semID;
  int sem_len = SLOTSMEMORIA + 2;
  int a = 0, b=-1, c=1, contador=0;
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
    printf("Semaforos creados bien shidori\n");
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
    for(contador = 0; contador < SLOTSMEMORIA; contador++){  
          if(c==4){
            exit(0);
          }
        b = semctl(semID, contador + 2, GETVAL, NULL);
        if(b==1 && (ZC_local+contador)->estado == 1){//1 significa semaforo verde y puedo escribir
          wait1(contador+2,semID);//pone el semaforo en rojo
          (ZC_local+contador)->estado = 0;//estado = no puedo escribir
          for(b=0;b<9;b++){
            (ZC_local+contador)->espacio[b] = atoi(argv[2]);
          }
          signal1(contador+2,semID);
          c++;

        }
        else{
          continue;
        }
      }
    }
  }
  else if(atoi(argv[1])==CONSUMIDOR){
    c=0;
  while(1){
    for(int contador = 0; contador < SLOTSMEMORIA; contador++){
      b = semctl(semID, contador + 2, GETVAL, NULL);
      if(b==1 && (ZC_local+contador)->estado == 0){
        printf("Zona[%d] Estado:%d Datos:%s\n", contador, (ZC_local+contador)->estado, (ZC_local+contador)->espacio);
        (ZC_local+contador)->estado == 1;
        c++;
        if(c==6){
          semctl(semID, 0, IPC_RMID);
  shmctl(mem_sharedID, IPC_RMID, 0);
          exit(0);
        }
      }
    }
  }   
  
  }
  else{
    printf("No args\n");
    exit(0);
  }
  return 0;
}
