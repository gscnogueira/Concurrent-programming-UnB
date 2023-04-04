/* Aluno: Gabriel da Silva Corvino Nogueira */
/* Matricula: 180113330 */
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

#define N 5
#define ESQ(id) (id + N - 1) % N
#define DIR(id)	(id+1) % N
#define FOME 0
#define COMENDO 1
#define PENSANDO 2


sem_t s[N];
sem_t mutex;
int estado[N] = {PENSANDO,PENSANDO,PENSANDO,PENSANDO,PENSANDO };

void *filosofos (void *arg);

void pega_talher (int n);
void devolve_talher (int n);

int main () {
	int i;
	int *id;

	// mutex
	sem_init(&mutex, 0, 1);

	//semaforo 
	for (i=0; i<N; i++) {
		sem_init(&s[i], 0, 0);
	}
	pthread_t r[N]; 

	//criacao das threads de filosofos
       for (i = 0; i < N ; i++) {
	     id = (int *) malloc(sizeof(int));
	     *id = i;
	     pthread_create(&r[i], NULL, filosofos, (void*) (id));
	}

        pthread_join(r[0],NULL);
	return 0;
}


void *filosofos (void *arg) {
	int n = *((int *) arg);
	while(1) {
		//pensar
		printf("Filosofo %d pensando ...\n", n);
		sleep(3);
		sem_wait(&mutex);
		estado[n] = FOME;
		sem_post(&mutex);
		pega_talher(n);
		//comer
		sem_wait(&mutex);
		estado[n] = COMENDO;
		sem_post(&mutex);
		printf("\tFilosofo %d comendo ...\n", n);
		sleep(3);
                printf("\tFilosofo %d acabou de comer ...\n", n);
		devolve_talher(n);
		sem_wait(&mutex);
		estado[n] = PENSANDO;
		sem_post(&mutex);
	} 
}

void testa(int n)
{
  sem_wait(&mutex);
  if(estado[n] == FOME &&
     estado[ESQ(n)] != COMENDO &&
     estado[DIR(n)] != COMENDO){
    
    estado[n]  = COMENDO;
    sem_post(&s[n]);

  }
  sem_post(&mutex);

}
void pega_talher (int n) {
  testa(n);
  sem_wait(&s[n]);
}

void devolve_talher (int n) {
  sem_wait(&mutex);
  estado[n] = PENSANDO;
  sem_post(&mutex);
  testa(ESQ(n));
  testa(DIR(n));
}

