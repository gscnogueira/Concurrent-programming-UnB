// Aluno: Gabriel da Silva Corvino Nogueira
// Matricula: 180113330
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define PR 1 //número de produtores
#define CN 1 // número de consumidores
#define N 4  //tamanho do buffer

int buffer = 0;
sem_t empty;
sem_t full;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * produtor(void * meuid);
void * consumidor (void * meuid);

void main(argc, argv)
	int argc;
	char *argv[];
{

	int erro;
	int i, n, m;
	int *id;

	pthread_t tid[PR];

	sem_init(&empty,0,N);    
	sem_init(&full,0,0);    

	for (i = 0; i < PR; i++)
	{
		id = (int *) malloc(sizeof(int));
		*id = i;
		erro = pthread_create(&tid[i], NULL, produtor, (void *) (id));

		if(erro)
		{
			printf("erro na criacao do thread %d\n", i);
			exit(1);
		}
	}

	pthread_t tCid[CN];

	for (i = 0; i < CN; i++)
	{
		id = (int *) malloc(sizeof(int));
		*id = i;
		erro = pthread_create(&tCid[i], NULL, consumidor, (void *) (id));

		if(erro)
		{
			printf("erro na criacao do thread %d\n", i);
			exit(1);
		}
	}

	pthread_join(tid[0],NULL);

}

void * produtor (void* pi)
{
	int id = *(int*)pi;


	while(1)
	{
		printf("Produtor %d está produzindo...\n", id);
		sleep(5);

		sem_wait(&empty);

		pthread_mutex_lock(&mutex);
		buffer++;
		printf("Produtor %d inseriu (buffer=%d)\n",id,  buffer);
		pthread_mutex_unlock(&mutex);

		 sem_post(&full);

	}
	pthread_exit(0);

}

void * consumidor (void* pi)
{
	int id = *(int*)pi;
	while(1)
	{
		sem_wait(&full);

		pthread_mutex_lock(&mutex);
		buffer--;
		printf("Consumidor %d retirou (buffer=%d)\n",id,  buffer);
		pthread_mutex_unlock(&mutex);

		 sem_post(&empty);

		printf("Consumidor %d está consumindo...\n",id,  buffer);
		sleep(10);


	}
	pthread_exit(0);

}

