#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define MA 10
#define MB 10
#define GA 1
#define GB 1


void * macacoA(void * meuid);
void * macacoB (void * meuid);
void * gorilaA (void * meuid);
void * gorilaB (void * meuid);


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ma_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t mb_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t ga_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t gb_cond = PTHREAD_COND_INITIALIZER;


int mA,mB,gA,gB = 0;

int gaQuer = 0;
int gbQuer = 0;
int maQuer = 0;
int mbQuer = 0;

int turno = 0; //0 para macacosA e 1 para macacosB

void main(argc, argv)
int argc;
char *argv[];
{

  int erro;
  int i, n, m;
  int *id;

  pthread_t tP[MA];
  for (i = 0; i < MA; i++)
  {
    id = (int *) malloc(sizeof(int));
    *id = i;
    erro = pthread_create(&tP[i], NULL, macacoA, (void *) (id));

    if(erro)
    {
      printf("erro na criacao do thread %d\n", i);
      exit(1);
    }
  }


  pthread_t tA[MB];
  for (i = 0; i < MB; i++)
  {
    id = (int *) malloc(sizeof(int));
    *id = i;
    erro = pthread_create(&tA[i], NULL, macacoB, (void *) (id));

    if(erro)
    {
      printf("erro na criacao do thread %d\n", i);
      exit(1);
    }
  }
  pthread_t tF[GA];
  for (i = 0; i < GA; i++){
    id = (int *) malloc(sizeof(int));
    *id = i;
    erro = pthread_create(&tF[i], NULL, gorilaA, (void *) (id));
    if(erro)
    {
      printf("erro na criacao do thread %d\n", i);
      exit(1);
    }
  }


 pthread_t tGB[GB];
  for (i = 0; i < GB; i++){
    id = (int *) malloc(sizeof(int));
    *id = i;
    erro = pthread_create(&tGB[i], NULL, gorilaB, (void *) (id));
    if(erro)
    {
      printf("erro na criacao do thread %d\n", i);
      exit(1);
    }
  }
  pthread_join(tP[0],NULL);
}	

void * macacoA(void * a){
    int i = *((int *) a);
    while(1){
	sleep(rand()%(i+1));
	pthread_mutex_lock(&mutex);
	maQuer++;
	 while(turno != 0 || gaQuer != 0 || gbQuer != 0 || mB > 0 || gA > 0 || gB > 0) {
	      pthread_cond_wait(&ma_cond,&mutex);
    	 }

	 mA++;
	maQuer--;
        if (mbQuer > 0){
	  turno = 1;	
	}
	pthread_mutex_unlock(&mutex);
	printf("Macaco %d passado de A para B \n",i);
	sleep(1);
	pthread_mutex_lock(&mutex);
	 mA--;
         printf("Macaco %d terminou de passar de A para B; num: %d\n" ,i,mA);
	 if(mA == 0){
         	pthread_cond_broadcast(&mb_cond);
         	pthread_cond_signal(&ga_cond);
         	pthread_cond_signal(&gb_cond);
	 }
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}

void * macacoB(void * a){
    int i = *((int *) a);
    while(1){
	 sleep(rand()%(i+1));
	 pthread_mutex_lock(&mutex);
 	 mbQuer++;
	  while(turno != 1 || gaQuer != 0 || gbQuer != 0 || mA > 0 || gA > 0 || gB > 0) {
	      pthread_cond_wait(&mb_cond,&mutex);
    	  }

	  mB++;
 	 mbQuer--;
         if(maQuer > 0){
	   turno = 0;
	}
	pthread_mutex_unlock(&mutex);
	printf("Macaco %d passado de B para A \n",i);
	sleep(1);
	pthread_mutex_lock(&mutex);
	 mB--;
         printf("Macaco %d terminou de passar de B para A; num: %d\n" ,i,mB);
	 if(mB == 0){
           pthread_cond_broadcast(&ma_cond);
           pthread_cond_signal(&ga_cond);
           pthread_cond_signal(&gb_cond);
         }
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}


void * gorilaA(void * a){
    int i = *((int *) a);
    while(1){
	 sleep(rand()%(i+1)+2);
	 pthread_mutex_lock(&mutex);
	  gaQuer++;
          while(mA > 0 || mB > 0 || gA > 0 || gB > 0) {
	      pthread_cond_wait(&ga_cond,&mutex);
    	  }
          gaQuer--;
          gA++;
	pthread_mutex_unlock(&mutex);
	printf("GORILA %d passado de A para B \n",i);
	sleep(5);
	pthread_mutex_lock(&mutex);
	 gA--;
         printf("GORILA %d terminou de passar de A para B; num: %d\n",i,gA);
	 pthread_cond_broadcast(&ma_cond);
         pthread_cond_broadcast(&mb_cond);
         pthread_cond_signal(&ga_cond);
         pthread_cond_signal(&gb_cond);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}



void * gorilaB(void * a){
    int i = *((int *) a);
    while(1){
	 sleep(rand()%(i+1)+10);
	 pthread_mutex_lock(&mutex);
	   gbQuer++;
           while(mA > 0 || mB > 0 || gA > 0 || gB > 0) {
	      pthread_cond_wait(&gb_cond,&mutex);
    	   }
           gbQuer--;
           gB++;
	pthread_mutex_unlock(&mutex);
	printf("GORILA %d passado de B para A \n",i);
	sleep(5);
	pthread_mutex_lock(&mutex);
	 gB--;
         printf("GORILA %d terminou de passar de B para A; num: %d\n" ,i,gB);
	 pthread_cond_broadcast(&ma_cond);
         pthread_cond_broadcast(&mb_cond);
         pthread_cond_signal(&ga_cond);
         pthread_cond_signal(&gb_cond);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}
