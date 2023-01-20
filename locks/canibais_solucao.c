#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAXCANIBAIS 25

void *canibal(void*meuid);
void cozinheiro(int m);

typedef struct {
  pthread_mutex_t mutex;
  pthread_cond_t cozinheiro;
  pthread_cond_t canibal;
  int comida;
} t_travessa;

t_travessa t = {PTHREAD_MUTEX_INITIALIZER, 
                PTHREAD_COND_INITIALIZER, 
                PTHREAD_COND_INITIALIZER, 
                0};


void main(argc, argv)
int argc;
char *argv[];
{
  int erro;
  int i, n, m;
  int *id;

  pthread_t tid[MAXCANIBAIS];

  if(argc != 3)
  {
    printf("erro na chamada do programa: jantar <#canibais> <#comida>\n");
    exit(1);
  }
  
  n = atoi (argv[1]);
  m = atoi (argv[2]);
  printf("numero de canibais: %d -- quantidade de comida: %d\n", n, m);

  if(n > MAXCANIBAIS)
  {
    printf("o numero de canibais e' maior que o maximo permitido: %d\n", MAXCANIBAIS);
    exit(1);
  }
  
  for (i = 0; i < n; i++)
  {
    id = (int *) malloc(sizeof(int));
    *id = i;
    erro = pthread_create(&tid[i], NULL, canibal, (void *) (id));

    if(erro)
    {
      printf("erro na criacao do thread %d\n", i);
      exit(1);
    }
  }

  cozinheiro(m);
}

void *canibal (void* pi){
  int contador;
  while(1)
  {
    pthread_mutex_lock(&(t.mutex)); 
    while(!t.comida) {
      printf("%d: vou esperar comida\n", *(int *)(pi));
      pthread_cond_wait(&(t.canibal),&(t.mutex));
    }
    printf("sou %d: vou me servir, tem %d porcoes\n", *(int *)(pi),t.comida);
    t.comida--;
    if (t.comida==0) {
        printf("%d: vou acordar o cozinheiro\n", *(int *)(pi));
        pthread_cond_signal(&(t.cozinheiro));
    }

    pthread_mutex_unlock(&(t.mutex)); 
    printf("%d: vou comer a porcao que peguei\n", *(int *)(pi));
    
    sleep(20);
  }
  
}

void cozinheiro (int m)
{
  int contador;
  while(1)
  {
    pthread_mutex_lock(&(t.mutex)); 
    
    while (t.comida) {
      printf("vou aguardar, tem %d porcoes\n", t.comida);
      pthread_cond_wait(&(t.cozinheiro),&(t.mutex));    
    }
    printf("vou cozinhar\n");
    sleep(10);
    t.comida += m;
    pthread_cond_broadcast(&(t.canibal));    
    pthread_mutex_unlock(&(t.mutex)); 
    
  
  }

}
