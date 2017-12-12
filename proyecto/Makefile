mapamemoria.o: C/mapa_memoria.c
	gcc -c C/mapa_memoria.c -lm -o OUT/mapamemoria.o -I H/
adminmemoria.o: C/admon_memoria.c
	gcc -c C/admon_memoria.c OUT/mapamemoria.o -o OUT/adminmemoria.o -I H/
admonmemoria: mapamemoria.o adminmemoria.o
	gcc -o OUT/admonmemoria OUT/adminmemoria.o OUT/mapamemoria.o -I H/ -lm
proyecto: C/main.c 
	gcc C/main.c -I H/ -o OUT/proyecto
miprompt: C/terminal.c
	gcc C/terminal.c -I H/ -o /tmp/miprompt
clean:
	rm OUT/* /tmp/miprompt