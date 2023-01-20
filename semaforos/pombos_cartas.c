#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "unistd.h"

#define N 10 //número de usuários

#define CARTAS 20 //quantidade de cartas na mochila

sem_t sem_mochila;
sem_t sem_voa_pombo;
sem_t sem_pombo;

int value = 20;
int cartas = 0;

void * f_usuario(void *arg);
void * f_pombo(void *arg);

int main(int argc, char **argv){
  int i;
  sem_init(&sem_mochila, 0, CARTAS);
  sem_init(&sem_pombo, 0, 1);
  sem_init(&sem_voa_pombo, 0, 0);

  pthread_t usuario[N];

  int *id;
  for(i = 0; i < N; i++){
    id = (int *) malloc(sizeof(int));
    *id = i;
    pthread_create(&(usuario[i]),NULL,f_usuario,  (void *) (id));
  }
  pthread_t pombo;
  id = (int *) malloc(sizeof(int));
  *id = 0;
  pthread_create(&(pombo),NULL,f_pombo, (void*) (id));

  pthread_join(pombo,NULL);

}


void * f_pombo(void *arg){

  while(1){
    printf("POMBO: esperando mochila ficar cheia\n");
    //Inicialmente está em A, aguardar/dorme a mochila ficar cheia (20 cartas)
    sem_wait(&sem_voa_pombo);
    sem_wait(&sem_pombo);


    //Leva as cartas para B e volta para A
    printf("POMBO: levando as cartas para B... (%d cartas)\n", cartas);
    sleep(5);
    printf("POMBO: Voltando para A...\n");
    sleep(5);

    //Acordar os usuários   
    printf("POMBO: Cheguei em A\n");
    int n = CARTAS;
    while(n--){
      sem_post(&sem_mochila);
    }
    cartas=0;
    sem_post(&sem_pombo);
  }
}

void * f_usuario(void *arg){

  int id = *(int*) arg ;

  while(1){
    //Escreve uma carta
    printf("Usuário %d: Estou escrevendo uma carta...\n", id);
    sleep(3);


    //Caso o pombo não esteja em A ou a mochila estiver cheia, então dorme	
    //
    //Posta sua carta na mochila do pombo

    printf("Usuário %d: esperando...\n",id);

    sem_wait(&sem_mochila);
    sem_wait(&sem_pombo);


    sem_getvalue(&sem_mochila, &value);
    cartas++;
    printf("Usuário %d: depositei minha carta. (%d cartas)\n", id, cartas);
    if(!value){
      printf("Usuário %d: Acorda!\n", id);
      sem_post(&sem_voa_pombo);
      sleep(1);
    }
    sem_post(&sem_pombo);
  }
}
