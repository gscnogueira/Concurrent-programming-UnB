/* Nome: Gabriel da Silva Corvino Nogueira 
 * Matrícula: 180113330 
 * Email: 180113330@aluno.unb.br
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "unistd.h"

#define NA 10		//número habitantes da ilha A
#define NB 10		//número habitantes da ilha B
#define N_ANC_A 10	//número de ansiões da ilha A
#define N_ANC_B 10	//número de ansiões da ilha B

#define CAPACIDADE_A  15 // Capacidade do navio para bens produzidos na ilha A
#define CAPACIDADE_B  15 // Capacidade do navio para bens produzidos na ilha B

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t zarpar_A = PTHREAD_COND_INITIALIZER;
pthread_cond_t habitantes_A = PTHREAD_COND_INITIALIZER;
pthread_cond_t zarpar_B = PTHREAD_COND_INITIALIZER;
pthread_cond_t habitantes_B = PTHREAD_COND_INITIALIZER;

int estoque_A=0;
int estoque_B=15;
int navio_em_a = 1;
int navio_em_b = 0;

void * f_habitanteA(void *arg);
void * f_ansiaoA(void *arg);
void * f_habitanteB(void *arg);
void * f_ansiaoB(void *arg);
void * f_navio(void *arg);

int main(int argc, char **argv){
  int i;
  int *id;

  // cria threads para habitantes da ilha A
  pthread_t habitantesA[NA];


  for(i = 0; i < NA; i++){
    id = (int *) malloc(sizeof(int));
    *id = i;
    pthread_create(&(habitantesA[i]),NULL,f_habitanteA,  (void *) (id));
  }


  // cria threads para habitantes da ilha B
  pthread_t habitantesB[NB];
  
  for(i = 0; i < NB; i++){
    id = (int *) malloc(sizeof(int));
    *id = i;
    pthread_create(&(habitantesB[i]),NULL,f_habitanteB,  (void *) (id));
  }
  
  //	// cria threads para anciões da ilha A
  //	pthread_t ancioesA[N];
  //
  //	for(i = 0; i < N_ANC_A; i++){
  //		id = (int *) malloc(sizeof(int));
  //		*id = i;
  //		pthread_create(&(usuario[i]),NULL,f_ansiaoA,  (void *) (id));
  //	}
  //
  //	// cria threads para anciões da ilha B
  //	pthread_t ancioesB[N];
  //
  //	for(i = 0; i < N_ANC_B; i++){
  //		id = (int *) malloc(sizeof(int));
  //		*id = i;
  //		pthread_create(&(usuario[i]),NULL,f_ansiaoB,  (void *) (id));
  //	}
  //

  // cria navio
  pthread_t navio;

  id = (int *) malloc(sizeof(int));
  *id = 0;
  pthread_create(&(navio),NULL,f_navio, (void*) (id));

  pthread_join(navio,NULL);

}


void * f_navio(void *arg){

  printf("Navio: atracado em em A\n");
  while(1){

    pthread_mutex_lock(&mutex);

    // Navio aguarda ficar cheio em A
    pthread_cond_wait(&zarpar_A, &mutex);

    // Navio parte para B
    printf("Navio: partindo (sentido A->B)\n");
    navio_em_a = 0;
    sleep(10); 

    // Navio chega em B
    printf("Navio: atracado em B\n");
    navio_em_b = 1;
    sleep(2);

    // Habitantes de B são liberados para abastecer o navio
    pthread_cond_broadcast(&habitantes_B);

    // Navio aguarda todas as trocas serem feitas para zarpar
    pthread_cond_wait(&zarpar_B, &mutex);

    // Navio parte para A
    printf("Navio: partindo (sentido B->A)\n");
    navio_em_b = 0;
    sleep(10);

    // Navio chega em A
    printf("Navio: atracado em A\n");
    navio_em_a = 1;
    sleep(2);

    // Habitantes de A são liberados para abastecer o navio
    pthread_cond_broadcast(&habitantes_A);

    pthread_mutex_unlock(&mutex);
  }
}

void * f_habitanteA(void *arg){

  int id = *((int*)arg);
  while(1){

    // Habitante colhe fruta
    printf("Habitante %d (ilha A): colhendo fruta...\n", id);
    sleep(id%5+5);

    pthread_mutex_lock(&mutex);
			

    // Caso o navio  esteja cheio ou não esteja em A, o habitante espera;
    while( estoque_A == CAPACIDADE_A || !navio_em_a || estoque_B == 0){
    printf("Habitante %d (ilha A): esperando...\n", id);
      pthread_cond_wait(&habitantes_A, &mutex);
    }

    // Habitante abastece navio navio
    estoque_A++;
    estoque_B--;
    printf("Habitante %d (ilha A): abastece navio com fruta (A:%d; B:%d)\n", id, estoque_A, estoque_B);


    // Caso navio atinja sua capacidade, ele é liberado para zarpar
    if ( estoque_A == CAPACIDADE_A || estoque_B == 0){
      pthread_cond_signal(&zarpar_A);
    }

    pthread_mutex_unlock(&mutex);
    sleep(3);
  }
}

void * f_habitanteB(void *arg){

  int id = *((int*)arg);
  while(1){

    // Habitante colhe fruta
    printf("Habitante %d (ilha B): colhendo fruta...\n", id);
    sleep(3);

    pthread_mutex_lock(&mutex);
			
    // Caso o não haja mais frutas para trocar, o  estoque esteja cheio ou  o navio não está em B, o habitante espera
    while( estoque_B == CAPACIDADE_B | !navio_em_b | estoque_A == 0){
      printf("Habitante %d (ilha B): esperando...\n", id);
      pthread_cond_wait(&habitantes_B, &mutex);
    }

    // Habitante abastece navio navio
    estoque_B++;
    estoque_A--;
    printf("Habitante %d (ilha B): abastece navio com fruta (A:%d; B:%d)\n", id, estoque_A, estoque_B);

    // Caso navio atinja sua capacidade, ele é liberado para zarpar
    if ( estoque_B == CAPACIDADE_B | estoque_A == 0){
      pthread_cond_signal(&zarpar_B);
    }

    pthread_mutex_unlock(&mutex);
    sleep(3);
  }

}

void * f_ansiaoA(void *arg){

  int id = *((int*)arg);
  while(1){

    // Habitante colhe fruta
    printf("Ansião %d (ilha A): colhendo fruta...\n", id);
    sleep(id%5+5);

    pthread_mutex_lock(&mutex);
			

    // Caso o navio  esteja cheio ou não esteja em A, o habitante espera;
    while( estoque_A == CAPACIDADE_A || !navio_em_a || estoque_B == 0){
    printf("Ansião %d (ilha A): esperando...\n", id);
      pthread_cond_wait(&habitantes_A, &mutex);
    }

    // Ansião abastece navio navio
    estoque_A++;
    estoque_B--;
    printf("Ansião %d (ilha A): abastece navio com fruta (A:%d; B:%d)\n", id, estoque_A, estoque_B);


    // Caso navio atinja sua capacidade, ele é liberado para zarpar
    if ( estoque_A == CAPACIDADE_A || estoque_B == 0){
      pthread_cond_signal(&zarpar_A);
    }

    pthread_mutex_unlock(&mutex);
    sleep(3);
  }
}

void * f_ansiaoB(void *arg){

}
