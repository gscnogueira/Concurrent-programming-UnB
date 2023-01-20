// Aluno: Gabriel da Silva Corvino Nogueira
// Matricula: 180113330
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define PR 1 //número de produtores
#define CN 1 // número de consumidores
#define N 4  //tamanho do buffer

int buffer = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;

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
 
  while(1)
  {
	  printf("Produtor está produzindo...\n");
	  sleep(10);

	  pthread_mutex_lock(&mutex);
	  while(buffer==N){
		  pthread_cond_signal(&cond2);
		  printf("produtor dormiu\n");
		  pthread_cond_wait(&cond1, &mutex);
	  }
	  buffer++;
	  printf("Produtor inseriu (buffer=%d)\n", buffer);
	  pthread_cond_signal(&cond2);
	  pthread_mutex_unlock(&mutex);


  }
  pthread_exit(0);
  
}

void * consumidor (void* pi)
{
 while(1)
  {
	  pthread_mutex_lock(&mutex);
	  while(buffer==0){
		  pthread_cond_signal(&cond1);
		  //printf("DORME CONSUMIDOR\n");
		  printf("consumidor dormiu\n");
		  pthread_cond_wait(&cond2, &mutex);
	  }
	  buffer--;
	  printf("Consumidor retirou (buffer=%d)\n", buffer);
	  pthread_cond_signal(&cond1);
	  pthread_mutex_unlock(&mutex);

	  printf("Consumidor está consumindo...\n", buffer);
	  sleep(2);
 
 
  }
  pthread_exit(0);
  
}
