#include <mapa_memoria.h>
#include <string.h>
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
		printf("lista vacia\n");
	}
	else{
		lista_tabla_pag.actual=lista_tabla_pag.inicial;
		while(lista_tabla_pag.actual->sig!=NULL){
		printf("ID proceso: %d Inicio_pagina: %d Fin_pagina: %d\n" ,lista_tabla_pag.actual->id_pag,lista_tabla_pag.actual->mem_fis[0],lista_tabla_pag.actual->mem_fis[1]);
		lista_tabla_pag.actual=lista_tabla_pag.actual->sig;
		}
		printf("ID proceso: %d Inicio_pagina: %d Fin_pagina: %d\n",lista_tabla_pag.actual->id_pag,lista_tabla_pag.actual->mem_fis[0],lista_tabla_pag.actual->mem_fis[1]);
		
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
void anadir_tabla_pag(int pid, int rango_init, int rango_fin){
 	lista_tabla_pag.aux = (struct tabla_pag *)malloc(sizeof(struct tabla_pag));
    lista_tabla_pag.aux->id_pag = pid;
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
		while(cola_p.actual->sig->sig!=NULL){
			cola_p.actual=cola_p.actual->sig;	
		}
		cola_p.actual->sig=cola_p.aux;
		cola_p.aux->sig=NULL;
		cola_p.actual=cola_p.aux;
	}
}

/*FUNCIONES PARA COLA DE PROCESOS*/
void mostrarcola(){
	if(cola_p.inicial==NULL){
		printf("No hay datos en la cola\n");
	}else{
		printf("Proceso mas viejo: %d\n",cola_p.inicial->p_id);
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
			anadir_tabla_pag(pid, inicio, fin);
			
}
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
void eliminar_mem(int pid){
	lista_log.aux=lista_log.inicial;
	while(lista_log.aux->sig!=NULL){
		if(lista_log.aux->process_id == pid){
			lista_log.aux->process_id = 0;
   			for(int d = 0;d<DATA_LEN;d++){
				lista_log.aux->datos[d] = 157;
			}
		}
		lista_log.aux=lista_log.aux->sig;
	}
	if(lista_log.aux->process_id == pid){
		lista_log.aux->process_id = 0;
   		for(int d = 0;d<DATA_LEN;d++){
			lista_log.aux->datos[d] = 157;
		}
	}
	delet_nodo(pid);
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
			reordenar_tabla_pag(lista_tabla_pag.aux->id_pag);
			lista_tabla_pag.aux=lista_tabla_pag.aux->sig;
		}
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
			inicial = lista_log.aux->process_id;
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