#include <sys/sem.h>
typedef struct{
  short estado;
  char espacio[10];
}zonacritica;
void wait1(int, int);
void signal1(int, int);