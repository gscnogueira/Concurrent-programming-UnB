#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>


#define NUMCARROS 2
#define NUMEQUIPES 5
#define CAPACIDADEPISTA 4

int qnt = 0;

pthread_t car[NUMCARROS*NUMEQUIPES];
sem_t equipes[NUMEQUIPES];
sem_t pista;
pthread_mutex_t l = PTHREAD_MUTEX_INITIALIZER;

void * piloto(void *arg);


int main(int argc, char **argv){
    int i;

    int *id;
    srand48(time(NULL));
    sem_init(&pista,0,CAPACIDADEPISTA);    
    for(i=0; i < NUMEQUIPES; i++){ 
        sem_init(&equipes[i],0,1);
    }
    for(i = 0; i < NUMCARROS*NUMEQUIPES;i++){
        id = (int *) malloc(sizeof(int));
        *id = i;
	pthread_create(&(car[i]),NULL,piloto, (void*) (id));
    }
   pthread_join(car[0],NULL);
  
}


void * piloto(void *arg){
    int id = *((int *) arg);
    int eq = id%NUMEQUIPES;
    printf("Carro %d da equipe %d criado\n",id,eq);
    while(1){

		sleep(drand48()*3);

		sem_wait(&equipes[eq]);
		sem_wait(&pista);
                
                pthread_mutex_lock(&l);
		   qnt++;
                printf("Carro %d treinando, quantidade: %d\n",id,qnt);
                pthread_mutex_unlock(&l);
                sleep(10*id);
             
                printf("Carro %d saindo da pista\n",id);
                pthread_mutex_lock(&l);
                   qnt--;
                pthread_mutex_unlock(&l); 
		sem_post(&pista);
                sem_post(&equipes[eq]);
                
		

    }
}




















