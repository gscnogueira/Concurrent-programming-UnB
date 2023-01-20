/* Aluno    : Gabriel da Silva Corvino Nogueira
 * Matricula: 180113330
 * */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAXCANIBAIS 20

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
int porcoes = 0;

void *canibal(void*meuid);
void *cozinheiro(int m);


void main(argc, argv)
int argc;
char *argv[];
{
  int erro;
  int i, n, m;
  int *id;

  pthread_t tid[MAXCANIBAIS];

  if(argc != 3){
    printf("erro na chamada do programa: jantar <#canibais> <#comida>\n");
    exit(1);
  }
  
  n = atoi (argv[1]); //número de canibais
  m = atoi (argv[2]); // quantidade de porções que o cozinheiro consegue preparar por vez
  printf("numero de canibais: %d -- quantidade de comida: %d\n", n, m);

  if(n > MAXCANIBAIS){
    printf("o numero de canibais e' maior que o maximo permitido: %d\n", MAXCANIBAIS);
    exit(1);
  }
  
  for (i = 0; i < n; i++)  {
    id = (int *) malloc(sizeof(int));
    *id = i;
    erro = pthread_create(&tid[i], NULL, canibal, (void *) (id));

    if(erro){
      printf("erro na criacao do thread %d\n", i);
      exit(1);
    }
  }

  cozinheiro(m);
}

void * canibal (void* pi){

  while(1) {
    //pegar uma porção de comida e acordar o cozinheiro se as porções acabaram
    pthread_mutex_lock(&lock);
    while(porcoes<=0){
        printf("canibal %d: aff ta sem...\n", *(int *)(pi));
        pthread_cond_signal(&cond2);
        pthread_cond_wait(&cond1, &lock);
    }
    printf("canibal %d: peguei meu ensopado (￣﹃￣)\n", *(int *)(pi));
    sleep(1);
    porcoes--;
    pthread_mutex_unlock(&lock);

    printf("canibal %d: comendo...\n", *(int *)(pi));
    sleep(5);
  }
  
}

void *cozinheiro (int m){
 
  while(1){
    printf("cozinheiro: vou cozinhar...\n");
    sleep(10);
    pthread_mutex_lock(&lock);
        printf("cozinheiro: terminei (╬ ಠ益ಠ)\n");
        porcoes+=m;
        while(porcoes==m){
            printf("cozinheiro: indo dormir\n");
            pthread_cond_broadcast(&cond1);
            pthread_cond_wait(&cond2, &lock);
        }
    pthread_mutex_unlock(&lock);
   }

}
