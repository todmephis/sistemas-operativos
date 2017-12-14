#include <pipas.h>
int main(void){
	int opt = -1;
	int fd;
	if((fd=mkfifo(PATH, 0666)) == -1){
	 	//perror("Error en el fifo");
	 	unlink(PATH);
	 	fd=mkfifo(PATH, 0666);
  	}
  	struct utsname sys_info;
	uname(&sys_info);
	//printf("%s\n", sys_info.sysname);
	if(strcmp(sys_info.sysname, MACOS) == 0)//Si el Kernel es Darwin
		system("osascript -e 'tell app \"Terminal\" to do script \"~/Desktop/proyectoSO/OUT/admonmemoria\"'");
	else if(strcmp(sys_info.sysname, LINUX) == 0)//Si el kernel es Linux
		system("/usr/bin/x-terminal-emulator -e ~/Escritorio/proyectoSO/OUT/admonmemoria");
  	else{
  		fprintf(stderr, "Kernel %s no soportado\n", sys_info.sysname);//Kernel desconocido
  		exit(EXIT_FAILURE);
  	}
  	if((fd=open(PATH, O_WRONLY)) == -1){
    	perror("Algo salio mal");
    	exit(EXIT_FAILURE);
  	}
	while(1){
		printf("1.- Crear procesos\n");
		printf("2.- Eliminar procesos\n");
		printf("3.- Fragmentar memoria\n");
		printf("4.- Mostrar memoria y tabla de pagina\n");
		printf("5.- Mostrar swap y tabla de pagina swap\n");
		printf("6.- Mostrar cola de procesos\n");
		printf("7.- Recuperar procesos de swap\n");
		printf(">> ");
		scanf("%d", &opt);
		system("clear");
		write(fd, &opt, sizeof(opt));
		if(opt == -1){
			unlink(PATH);
			exit(EXIT_SUCCESS);
		}          
	}
}