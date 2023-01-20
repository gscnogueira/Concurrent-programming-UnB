/* Aluno: Gabriel da Silva Corvino Nogueira
 * Matricula: 1800113330
 * Email: gab.nog94@gmail.com
 * */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MA 10 //macacos que andam de A para B
#define MB 10 //macacos que andam de B para A

pthread_mutex_t lock_ab   = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_ba   = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_rope = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_turn = PTHREAD_MUTEX_INITIALIZER;

int cnt_ab = 0, cnt_ba = 0;

void * macacoAB(void * a) {
	int i = *((int *) a);    
	while(1){
		//Procedimentos para acessar a corda

		pthread_mutex_lock(&lock_turn);
		pthread_mutex_lock(&lock_ab);
			cnt_ab++;
			if (cnt_ab == 1)
				pthread_mutex_lock(&lock_rope);
		pthread_mutex_unlock(&lock_ab);
		pthread_mutex_unlock(&lock_turn);

		printf("Macaco %d passado de A para B [AB->%d, BA->%d]\n",i, cnt_ab, cnt_ba);
		sleep(1);

		pthread_mutex_lock(&lock_ab);
			cnt_ab--;
			if (cnt_ab == 0)
				pthread_mutex_unlock(&lock_rope);
		pthread_mutex_unlock(&lock_ab);
		//Procedimentos para quando sair da corda
	}
	pthread_exit(0);
}

void * macacoBA(void * a) {
	int i = *((int *) a);    
	while(1){
		//Procedimentos para acessar a corda

		pthread_mutex_lock(&lock_turn);
		pthread_mutex_lock(&lock_ba);
			cnt_ba++;
			if(cnt_ba == 1)
				pthread_mutex_lock(&lock_rope);
		pthread_mutex_unlock(&lock_ba);
		pthread_mutex_unlock(&lock_turn);

		printf("Macaco %d passado de B para A [AB->%d, BA->%d]\n",i, cnt_ab, cnt_ba);
		sleep(1);

		pthread_mutex_lock(&lock_ba);
			cnt_ba--;
			if(cnt_ba == 0)
				pthread_mutex_unlock(&lock_rope);
		pthread_mutex_unlock(&lock_ba);

		//Procedimentos para quando sair da corda
	}
	pthread_exit(0);
}



void * gorila(void * a){
	while(1){
		//Procedimentos para acessar a corda
		pthread_mutex_lock(&lock_turn);
		pthread_mutex_lock(&lock_rope);

		pthread_mutex_unlock(&lock_turn);

		printf("Gorila passado de A para B \n");
		sleep(5);
		pthread_mutex_unlock(&lock_rope);
		//Procedimentos para quando sair da corda
	}
	pthread_exit(0);
}

int main(int argc, char * argv[])
{
	pthread_t macacos[MA+MB];
	int *id;
	int i = 0;

	for(i = 0; i < MA+MB; i++){
		id = (int *) malloc(sizeof(int));
		*id = i;
		if(i%2 == 0){
			if(pthread_create(&macacos[i], NULL, &macacoAB, (void*)id)){
				printf("Não pode criar a thread %d\n", i);
				return -1;
			}
		}else{
			if(pthread_create(&macacos[i], NULL, &macacoBA, (void*)id)){
				printf("Não pode criar a thread %d\n", i);
				return -1;
			}
		}
	}
	pthread_t g;
	pthread_create(&g, NULL, &gorila, NULL);


	pthread_join(macacos[0], NULL);
	return 0;
}
