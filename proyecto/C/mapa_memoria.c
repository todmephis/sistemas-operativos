#include <mapa_memoria.h>
#include <string.h>
#include <unistd.h>
/*FUNCIONES PARA GESTIONAR MEMORIA*/
void mostrar(void){
	if(lista_log.inicial==NULL){
		printf("lista vacia\n");
	}
	else{
		lista_log.actual=lista_log.inicial;
		while(lista_log.actual->sig!=NULL){
		printf("ID pagina: %d Contenido: %s PID: %d\n",lista_log.actual->id_pag,lista_log.actual->datos,lista_log.actual->process_id);
		lista_log.actual=lista_log.actual->sig;
		}
	}
}
void mostrar_tabla_pag(void){
	if(lista_tabla_pag.inicial==NULL){
		printf("Tabla de paginas vacia\n");
	}
	else{
		lista_tabla_pag.actual=lista_tabla_pag.inicial;
		while(lista_tabla_pag.actual->sig!=NULL){
			printf("ID proceso: %d Inicio_pagina: %d Fin_pagina: %d " ,lista_tabla_pag.actual->id_pag,lista_tabla_pag.actual->mem_fis[0],lista_tabla_pag.actual->mem_fis[1]);
			if(lista_tabla_pag.actual->type_mem == LOGICA){
				printf("Lugar actual: Mem Logica");
			}
			else if(lista_tabla_pag.actual->type_mem == SWAP){
				printf("Lugar actual: Mem SWAP");
			}
			printf("\n");
			lista_tabla_pag.actual=lista_tabla_pag.actual->sig;
		}
		printf("ID proceso: %d Inicio_pagina: %d Fin_pagina: %d ",lista_tabla_pag.actual->id_pag,lista_tabla_pag.actual->mem_fis[0],lista_tabla_pag.actual->mem_fis[1]);
		if(lista_tabla_pag.actual->type_mem == LOGICA){
			printf("Lugar actual: Mem Logica");
		}
		else if(lista_tabla_pag.actual->type_mem == SWAP){
			printf("Lugar actual: Mem SWAP");
		}
		printf("\n");
	}
}
int mem_disp(void){
	int memoria = 0;
		lista_log.actual=lista_log.inicial;
		while(lista_log.actual->sig!=NULL){
			if(lista_log.actual->process_id == 0){
				memoria++;
			}
		lista_log.actual=lista_log.actual->sig;
		}
		if(lista_log.actual->process_id == 0){
			memoria++;
		}
		return memoria;
}
void anadir_tabla_pag(int pid, int rango_init, int rango_fin, int type){
 	lista_tabla_pag.aux = (struct tabla_pag *)malloc(sizeof(struct tabla_pag));
    lista_tabla_pag.aux->id_pag = pid;
    lista_tabla_pag.aux-> type_mem = type;
    lista_tabla_pag.aux->mem_fis[0]=rango_init;
    lista_tabla_pag.aux->mem_fis[1]=rango_fin;
	lista_tabla_pag.aux->sig=NULL;
	lista_tabla_pag.aux->ant=NULL;
	if(lista_tabla_pag.inicial==NULL){
		lista_tabla_pag.inicial=lista_tabla_pag.aux;
	}
	else{
		lista_tabla_pag.actual=lista_tabla_pag.inicial;
		while(lista_tabla_pag.actual->sig!=NULL){
			lista_tabla_pag.actual=lista_tabla_pag.actual->sig;	
		}
		lista_tabla_pag.actual->sig=lista_tabla_pag.aux;
		lista_tabla_pag.aux->ant=lista_tabla_pag.actual;
		lista_tabla_pag.aux->sig=NULL;
		lista_tabla_pag.actual=lista_tabla_pag.aux;
	}
}
void init_mem_log(int id_p){
    lista_log.aux = (struct nodoLogica *)malloc(sizeof(struct nodoLogica));
    lista_log.aux->id_pag = id_p;
    lista_log.aux->process_id=0;
    for(int d = 0;d<DATA_LEN;d++){
		lista_log.aux->datos[d] = 157;
	}
	lista_log.aux->sig=NULL;
	lista_log.aux->ant=NULL;
	if(lista_log.inicial==NULL){
		lista_log.inicial=lista_log.aux;
	}
	else{
		lista_log.actual=lista_log.inicial;
		while(lista_log.actual->sig!=NULL){
			lista_log.actual=lista_log.actual->sig;	
		}
		lista_log.actual->sig=lista_log.aux;
		lista_log.aux->ant=lista_log.actual;
		lista_log.aux->sig=NULL;
		lista_log.actual=lista_log.aux;
	}
}
void encolar(int process){
	cola_p.aux = (struct orden_process*)malloc(sizeof(struct orden_process));
    cola_p.aux->p_id = process;
	cola_p.aux->sig=NULL;
	if(cola_p.inicial==NULL){
		cola_p.inicial=cola_p.aux;
	}
	else{
		cola_p.actual=cola_p.inicial;
		while(cola_p.actual->sig!=NULL){
			cola_p.actual=cola_p.actual->sig;	
		}
		cola_p.actual->sig=cola_p.aux;
		cola_p.aux->sig=NULL;
		cola_p.actual=cola_p.aux;
	}
}

/*FUNCIONES PARA COLA DE PROCESOS*/
int mostrarcola(){
	if(cola_p.inicial==NULL){
		printf("No hay datos en la cola\n");
	}else{
		printf("Proceso mas viejo: %d\n",cola_p.inicial->p_id);
		return cola_p.inicial->p_id;
	}
}
void desencolar(){
	if(cola_p.inicial==NULL){
		printf("No hay datos en la cola\n");
	}else{
		if(cola_p.actual==cola_p.inicial){
			free(cola_p.inicial);
		}
		else{
			cola_p.aux=cola_p.inicial;
			cola_p.inicial=cola_p.inicial->sig;
			free(cola_p.aux);
		}
	}
}
/*FUNCIONES DE OPERACION DEL ADMON DE MEMORIA*/
void delet_nodo(int id){
	int cont = 0;
	lista_tabla_pag.actual = lista_tabla_pag.inicial;
	while(lista_tabla_pag.actual->sig!=NULL){
		lista_tabla_pag.actual=lista_tabla_pag.actual->sig;
	}
	if(lista_tabla_pag.inicial->id_pag==id && lista_tabla_pag.inicial->sig==NULL){
		free(lista_tabla_pag.inicial);
		lista_tabla_pag.inicial = NULL;
		lista_tabla_pag.aux = NULL;
		lista_tabla_pag.actual = NULL;
	}
	else if(lista_tabla_pag.inicial->id_pag==id && lista_tabla_pag.inicial->sig!=NULL){
		lista_tabla_pag.aux = lista_tabla_pag.inicial;
		lista_tabla_pag.inicial = lista_tabla_pag.inicial->sig;
		lista_tabla_pag.inicial->ant=NULL;
		free(lista_tabla_pag.aux);
	}
	else if(lista_tabla_pag.actual->id_pag==id && lista_tabla_pag.actual->sig==NULL){
		lista_tabla_pag.aux = lista_tabla_pag.actual;
		lista_tabla_pag.actual = lista_tabla_pag.actual->ant;
		lista_tabla_pag.inicial->sig=NULL;
		free(lista_tabla_pag.aux);
	}
	else{
		lista_tabla_pag.aux = lista_tabla_pag.inicial;
		while(lista_tabla_pag.aux->sig!=NULL){
			if(lista_tabla_pag.aux->id_pag == id){
				lista_tabla_pag.aux->ant->sig = lista_tabla_pag.aux->sig;
				lista_tabla_pag.aux->sig->ant = lista_tabla_pag.aux->ant;
				free(lista_tabla_pag.aux);
				break;
			}
			lista_tabla_pag.aux=lista_tabla_pag.aux->sig;
		}
	}
}
int eliminar_mem(int pid){
	/////////////////////////////////////////////LOGICA
	int bandera = 1;
	lista_log.aux=lista_log.inicial;
	while(lista_log.aux->sig!=NULL){
		if(lista_log.aux->process_id == pid){
			lista_log.aux->process_id = 0;
   			for(int d = 0;d<DATA_LEN;d++){
				lista_log.aux->datos[d] = 157;
			}
			if(bandera == 1){
				delet_nodo(pid);
				desencolar();
				bandera = 0;
			}
		}
		lista_log.aux=lista_log.aux->sig;
	}
	if(lista_log.aux->process_id == pid){

		lista_log.aux->process_id = 0;
   		for(int d = 0;d<DATA_LEN;d++){
			lista_log.aux->datos[d] = 157;
		}
		if(bandera == 1){
			delet_nodo(pid);
			desencolar();
			bandera = 0;
		}
	}
	if(bandera==0){
		return 0;
	}
	bandera = 1;
	//////////////////////////////////////////////SWAP
	swap.aux=swap.inicial;
	while(swap.aux->sig!=NULL){
		if(swap.aux->process_id == pid){
			swap.aux->process_id = 0;
   			for(int d = 0;d<DATA_LEN;d++){
				swap.aux->datos[d] = 157;
			}
			if(bandera == 1){
				delet_nodo(pid);
				bandera = 0;
			}
		}
		swap.aux=swap.aux->sig;
	}
	if(swap.aux->process_id == pid){
		swap.aux->process_id = 0;
   		for(int d = 0;d<DATA_LEN;d++){
			swap.aux->datos[d] = 157;
		}
		if(bandera == 1){
			delet_nodo(pid);
			bandera = 0;
		}
	}
}
void init_swap(int id_p){
	swap.aux = (struct nodoLogica *)malloc(sizeof(struct nodoLogica));
    swap.aux->id_pag = id_p;
    swap.aux->process_id=0;
    for(int d = 0;d<DATA_LEN;d++){
		swap.aux->datos[d] = 157;
	}
	swap.aux->sig=NULL;
	swap.aux->ant=NULL;
	if(swap.inicial==NULL){
		swap.inicial=swap.aux;
	}
	else{
		swap.actual=swap.inicial;
		while(swap.actual->sig!=NULL){
			swap.actual=swap.actual->sig;	
		}
		swap.actual->sig=swap.aux;
		swap.aux->ant=swap.actual;
		swap.aux->sig=NULL;
		swap.actual=swap.aux;
	}
}
void mostrar_swap(void){
	if(swap.inicial==NULL){
		printf("lista vacia\n");
	}
	else{
		swap.actual=swap.inicial;
		while(swap.actual->sig!=NULL){
		printf("ID pagina: %d Contenido: %s PID: %d\n",swap.actual->id_pag,swap.actual->datos,swap.actual->process_id);
		swap.actual=swap.actual->sig;
		}
	}
}
int mem__swap_disp(void){
	int memoria = 0;
		swap.actual=swap.inicial;
		while(swap.actual->sig!=NULL){
			if(swap.actual->process_id == 0){
				memoria++;
			}
		swap.actual=swap.actual->sig;
		}
		if(swap.actual->process_id == 0){
			memoria++;
		}
		return memoria;
}
int mover_swap(void){
	int mem, id_proceso,inicio,final, longitud, pid;
	char data[DATA_LEN];
	pid = mostrarcola();
	////////////////////////////////////////////////////// Revisa disponibilidad de memoria en swap
	lista_tabla_pag.aux=lista_tabla_pag.inicial;
	while(lista_tabla_pag.aux->sig!=NULL){
		if(lista_tabla_pag.aux->id_pag == pid){
			inicio = lista_tabla_pag.aux->mem_fis[0];
			final = lista_tabla_pag.aux->mem_fis[1];
			id_proceso = pid;
			mem = lista_tabla_pag.aux->mem_fis[1]-lista_tabla_pag.aux->mem_fis[0] +1;
			printf("Revisando si existen (%d) paginas disponibles en swap\n", mem);
			if(mem__swap_disp() < mem){
				printf("error, Memoria swap disponible: %d\n", mem__swap_disp()-1);
				return 1;
			}
		}
		lista_tabla_pag.aux=lista_tabla_pag.aux->sig;
	}
	if(lista_tabla_pag.aux->id_pag == pid){
		inicio = lista_tabla_pag.aux->mem_fis[0];
		final = lista_tabla_pag.aux->mem_fis[1];
		id_proceso = pid;
		mem = lista_tabla_pag.aux->mem_fis[1]-lista_tabla_pag.aux->mem_fis[0];
		printf("Revisando si existen (%d) paginas disponibles en swap\n", mem);
		if(mem__swap_disp() < mem){
			printf("error, Memoria swap disponible: %d\n", mem__swap_disp()-1);
			return 1;
		}
	}
	printf("Memoria swap disponible\n");
	longitud = final - inicio;
	////////////////////////////////////////////////////// Copia de lógica a swap
	lista_log.aux = lista_log.inicial;
	while(lista_log.aux->sig != NULL){
		if(lista_log.aux->process_id == pid){
			memcpy(data,lista_log.aux->datos, (int)sizeof(lista_log.aux->datos));
			break;
		}
		lista_log.aux=lista_log.aux->sig;
	}
	if(lista_log.aux->process_id == pid){
		memcpy(data,lista_log.aux->datos, (int)sizeof(lista_log.aux->datos));
	}
	//printf("Datos: %s\n", data);;
	eliminar_mem(pid);
	insertar_swap(data[0], mem, pid);
	/////////////////////////////////////////////////////
}
void insertar_mem(char letra, float tam, int pid){
	int contador=1;
	int inicio,fin;
	lista_log.aux=lista_log.inicial;
	while(lista_log.aux->sig!=NULL){
		if(lista_log.aux->process_id==0){
			fin = lista_log.aux->id_pag;
			if(contador==(int)ceil(tam/4)){
				for(int c=0;c<(int)ceil(tam/4);c++){
					for(int d = 0;d<DATA_LEN;d++){
						lista_log.aux->datos[d] = letra;
					}
					//printf("Indice de nodo: %d\n",lista_log.aux->id_pag);
					lista_log.aux->process_id=pid;
					lista_log.aux=lista_log.aux->ant;	
				}
				break;
			}
			contador++;
		}else{
			contador = 1;
		}
		lista_log.aux=lista_log.aux->sig;
	}
	inicio = fin - (int)ceil(tam/4)+1;
	encolar(pid);
	anadir_tabla_pag(pid, inicio, fin, LOGICA);		
}
void insertar_swap(char letra, int tam, int pid){
	int contador=1;
	int inicio,fin;
	swap.aux=swap.inicial;
	while(swap.aux->sig!=NULL){
		if(swap.aux->process_id==0){
			fin = swap.aux->id_pag;
			if(contador == tam){
				for(int c=0;c < tam;c++){
					for(int d = 0;d < DATA_LEN;d++){
						swap.aux->datos[d] = letra;
					}
					//printf("Indice de nodo: %d\n",lista_log.aux->id_pag);
					swap.aux->process_id=pid;
					swap.aux=swap.aux->ant;	
				}
				break;
			}
			contador++;
		}else{
			contador = 1;
		}
		swap.aux=swap.aux->sig;
	}
	inicio = fin - tam+1;
	anadir_tabla_pag(pid, inicio, fin, SWAP);		
}
void fragmentar_swap(void){
	swap.aux=swap.inicial->sig;
	while(swap.aux->sig!=NULL){
		while(swap.aux->ant->process_id == 0 && swap.aux->process_id != 0){
			swap.aux->ant->process_id = swap.aux->process_id;
			swap.aux->process_id = 0;
			memcpy(swap.aux->ant->datos,swap.aux->datos, (int)sizeof(swap.aux->datos));
			for(int d = 0;d<DATA_LEN;d++){
				swap.aux->datos[d] = 157;
			}
			swap.aux=swap.aux->ant;
			if(swap.aux == swap.inicial){
				break;
			}
		}
		swap.aux=swap.aux->sig;
	}
	while(swap.aux->ant->process_id == 0 && swap.aux->process_id != 0){
		swap.aux->ant->process_id = swap.aux->process_id;
		swap.aux->process_id = 0;
		memcpy(swap.aux->ant->datos,swap.aux->datos, (int)sizeof(swap.aux->datos));
		for(int d = 0;d<DATA_LEN;d++){
			swap.aux->datos[d] = 157;
		}
		swap.aux=swap.aux->ant;
		if(swap.aux == swap.inicial){
			break;
		}
	}
	printf("Fragmentación Correcta\n");
	/////////////////////////////////////////////////////////// tabla de pag
	if(lista_tabla_pag.inicial == NULL){
		printf("No hay procesos\n");
	} 
	else{
		lista_tabla_pag.aux = lista_tabla_pag.inicial;
		while(lista_tabla_pag.aux->sig!=NULL){
			if(lista_tabla_pag.aux->type_mem == SWAP)
				reordenar_tabla_pag_swap(lista_tabla_pag.aux->id_pag);
			lista_tabla_pag.aux=lista_tabla_pag.aux->sig;
		}
		if(lista_tabla_pag.aux->type_mem == SWAP)
			reordenar_tabla_pag_swap(lista_tabla_pag.aux->id_pag);
	}
}
void reordenar_tabla_pag_swap(int pid){
	int inicial = 0, final = 0, bandera = 0, contador = 0;
	swap.aux = swap.inicial;
	while(swap.aux->sig != NULL){
		if(swap.aux->process_id == pid){
			if(bandera == 0){
				inicial = swap.aux->id_pag;
				bandera = 1;
			}
			contador++;
		}
		swap.aux=swap.aux->sig;
	}
	if(swap.aux->process_id == pid){
		if(bandera == 0){
			inicial = swap.aux->id_pag;
			bandera = 1;
		}
		contador++;
	}
	final = inicial + contador -1;
	///////////////////////////////////////////////////////////
	lista_tabla_pag.aux2 = lista_tabla_pag.inicial;
	while(lista_tabla_pag.aux2->sig!=NULL){
		if(lista_tabla_pag.aux2->id_pag == pid){
			lista_tabla_pag.aux2->mem_fis[0]=inicial;
			lista_tabla_pag.aux2->mem_fis[1]=final;
		}
		lista_tabla_pag.aux2=lista_tabla_pag.aux2->sig;	
	}
	if(lista_tabla_pag.aux2->id_pag == pid){
		lista_tabla_pag.aux2->mem_fis[0]=inicial;
		lista_tabla_pag.aux2->mem_fis[1]=final;
	}
}
void fragmentar(void){
	lista_log.aux=lista_log.inicial->sig;
	while(lista_log.aux->sig!=NULL){
		while(lista_log.aux->ant->process_id == 0 && lista_log.aux->process_id != 0){
			lista_log.aux->ant->process_id = lista_log.aux->process_id;
			lista_log.aux->process_id = 0;
			memcpy(lista_log.aux->ant->datos,lista_log.aux->datos, (int)sizeof(lista_log.aux->datos));
			for(int d = 0;d<DATA_LEN;d++){
				lista_log.aux->datos[d] = 157;
			}
			lista_log.aux=lista_log.aux->ant;
			if(lista_log.aux == lista_log.inicial){
				break;
			}
		}
		lista_log.aux=lista_log.aux->sig;
	}
	while(lista_log.aux->ant->process_id == 0 && lista_log.aux->process_id != 0){
		lista_log.aux->ant->process_id = lista_log.aux->process_id;
		lista_log.aux->process_id = 0;
		memcpy(lista_log.aux->ant->datos,lista_log.aux->datos, (int)sizeof(lista_log.aux->datos));
		for(int d = 0;d<DATA_LEN;d++){
			lista_log.aux->datos[d] = 157;
		}
		lista_log.aux=lista_log.aux->ant;
		if(lista_log.aux == lista_log.inicial){
			break;
		}
	}
	printf("Fragmentación Correcta\n");
	/////////////////////////////////////////////////////////// tabla de pag
	if(lista_tabla_pag.inicial == NULL){
		printf("No hay procesos\n");
	} 
	else{
		lista_tabla_pag.aux = lista_tabla_pag.inicial;
		while(lista_tabla_pag.aux->sig!=NULL){
			if(lista_tabla_pag.aux->type_mem == LOGICA)
				reordenar_tabla_pag(lista_tabla_pag.aux->id_pag);
			lista_tabla_pag.aux=lista_tabla_pag.aux->sig;
		}
		if(lista_tabla_pag.aux->type_mem == LOGICA)
			reordenar_tabla_pag(lista_tabla_pag.aux->id_pag);
	}
}
void reordenar_tabla_pag(int pid){
	int inicial = 0, final = 0, bandera = 0, contador = 0;
	lista_log.aux = lista_log.inicial;
	while(lista_log.aux->sig != NULL){
		if(lista_log.aux->process_id == pid){
			if(bandera == 0){
				inicial = lista_log.aux->id_pag;
				bandera = 1;
			}
			contador++;
		}
		lista_log.aux=lista_log.aux->sig;
	}
	if(lista_log.aux->process_id == pid){
		if(bandera == 0){
			inicial = lista_log.aux->id_pag;
			bandera = 1;
		}
		contador++;
	}
	final = inicial + contador -1;
	///////////////////////////////////////////////////////////
	lista_tabla_pag.aux2 = lista_tabla_pag.inicial;
	while(lista_tabla_pag.aux2->sig!=NULL){
		if(lista_tabla_pag.aux2->id_pag == pid){
			lista_tabla_pag.aux2->mem_fis[0]=inicial;
			lista_tabla_pag.aux2->mem_fis[1]=final;
		}
		lista_tabla_pag.aux2=lista_tabla_pag.aux2->sig;	
	}
	if(lista_tabla_pag.aux2->id_pag == pid){
		lista_tabla_pag.aux2->mem_fis[0]=inicial;
		lista_tabla_pag.aux2->mem_fis[1]=final;
	}
}