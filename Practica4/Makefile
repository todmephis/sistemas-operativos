semaforos.o: C/semaforos.c
	gcc -c C/semaforos.c -o OUT/semaforos.o -I H/ -Wall
main.o: C/main.c
	gcc -c C/main.c -o OUT/main.o -I H/ -Wall
practica4: main.o semaforos.o
	gcc -o OUT/practica4 OUT/semaforos.o OUT/main.o -Wall
clean:
	rm -f OUT/*
