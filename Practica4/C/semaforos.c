#include <semaforos.h>
void wait1(int num_sem, int id){
	struct sembuf op;
	op.sem_num = num_sem;
	op.sem_op = -1;
	op.sem_flg = SEM_UNDO;
	semop(id, &op, 1);
}
void signal1(int num_sem, int id){
	struct sembuf op;
	op.sem_num = num_sem;
	op.sem_op = 1;
	op.sem_flg = SEM_UNDO;
	semop(id, &op, 1);
}