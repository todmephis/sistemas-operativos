#include <pipas.h>
#include <stdio.h>
#include <stdlib.h>
int main(void){
	int fd;
	char letra;
	printf("Letra >> ");
	scanf("%c", &letra);
	fd = open(PATH2, O_WRONLY);
  	write(fd, &letra, sizeof(letra));
  	close(fd);
  	unlink(PATH2);
	return 0;
}