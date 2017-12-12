#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#define DATA_LEN 5
struct nodoLogica{
	int process_id;
	short id_pag;
	char datos[DATA_LEN];
	struct nodoLogica *sig,*ant;	
};
struct tabla_pag{
	int id_pag;
	int mem_fis[2];
	struct tabla_pag *sig,*ant;
};
struct orden_process{
	int p_id;
	struct orden_process *sig;	
};
struct lista{
	struct nodoLogica *inicial, *actual, *aux;
};
struct lista2{
	struct tabla_pag *inicial, *actual, *aux, *aux2;
};
struct cola_proc{
	struct orden_process *inicial, *actual, *aux;
};
struct lista lista_log;
struct lista2 lista_tabla_pag;
struct cola_proc cola_p;

void mostrar(void);
int mem_disp(void);
void init_mem_log(int);
void insertar_mem(char, float, int);
void mostrar_tabla_pag(void);
void anadir_tabla_pag(int, int, int);
void encolar(int);
void mostrarcola(void);
void desencolar(void);
void eliminar_mem(int);
void delet_nodo(int);
void fragmentar(void);
void reordenar_tabla_pag(int);