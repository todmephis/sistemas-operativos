#include <admon_memoria.h>
#include <pipas.h>
int main(void){
	int buff, fd = -1, fd2, temp;;
	float size, memoria_disp;
	char subfix, letra;
	struct utsname sys_info;
  	if((fd=open(PATH, O_RDONLY)) == -1){
		perror("Algo salio mal");
	}
	printf("Acmon de memoria bergas :v\n");
	for(int a =0; a<513;a++){
		init_mem_log(a);
	}
	//printf("%s\n", sys_info.sysname);
	while(1){
	read(fd, &buff, sizeof(buff));
	//printf("%d\n", buff);
	switch(buff){
		case 1:
			uname(&sys_info);
			srand(time(NULL));
			memoria_disp = mem_disp()-1;
			printf("Tamaño del proceso [TAMm/k] >>  ");
			scanf("%f%c",&size,&subfix);
			//printf("Tamaño deseado(m,k): %d%c\n",size,subfix);
			if(subfix == 'm'){
				float mb = (memoria_disp * 4) / KB_SIZE;
				if(size > 2){
					printf("Tamaño total de memoria exedido\n");
				}
				else if(size > mb){
					printf("Memoria insuficiente\n");
				}
				else{
					if(strcmp(sys_info.sysname, MACOS) == 0)//Si el kernel es Darwin
						system("osascript -e 'tell app \"Terminal\" to do script \"/tmp/miprompt\"'");
					else if(strcmp(sys_info.sysname, LINUX) == 0)//Si el kernel es Linux
						system("/usr/bin/x-terminal-emulator -e /tmp/miprompt");	
					unlink(PATH2);
					fd2=mkfifo(PATH2, 0666);
					fd2=open(PATH2, O_RDONLY);
					read(fd2, &letra, sizeof(letra));
					close(fd2);
					printf("letra %c\n", letra);
					insertar_mem(letra, size*KB_SIZE, rand()% 5000 + 1);
					//mostrar();
					//mostrar_tabla_pag();
				}
			}
			else if(subfix == 'k'){
				int kb = memoria_disp * 4; 
				if(size > 2048){
					printf("Tamaño total de memoria excedido\n");
				}
				else if(size > kb){
					printf("Memoria insuficiente\n");
				}
				else{
					if(strcmp(sys_info.sysname, MACOS) == 0)//Si el kernel es Darwin
						system("osascript -e 'tell app \"Terminal\" to do script \"/tmp/miprompt\"'");
					else if(strcmp(sys_info.sysname, LINUX) == 0)//Si el kernel es Linux
						system("/usr/bin/x-terminal-emulator -e /tmp/miprompt");
					unlink(PATH2);
					fd2=mkfifo(PATH2, 0666);
					fd2=open(PATH2, O_RDONLY);
					read(fd2, &letra, sizeof(letra));
					close(fd2);
					printf("letra %c\n", letra);
					insertar_mem(letra, size, rand()% 5000 + 1);
					//mostrar();
					//mostrar_tabla_pag();
				}				
			}
			break;
		case 2:
			mostrar_tabla_pag();
			printf("Ingresa el ID del proceso que quieres eliminar\n");
			scanf("%d",&temp);
			eliminar_mem(temp);
			break;
		case 3:
			fragmentar();
			break;
		case 4:
			mostrar();
			mostrar_tabla_pag();
			break;
		case -1:
			exit(0);
			break;
		default:
			printf("Opcion invalida\n");
			break;
	}
	}
	//mostrar();
	return 0;
}