#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define PAC 10  
#define PBD 5   

pthread_barrier_t barrier;  

void * func_pac(void *arg) {
  int myid = *(int *)(arg);
  while(1){
     printf("A");
     pthread_barrier_wait(&barrier); //sinaliza que imprimiu A
     pthread_barrier_wait(&barrier); //aguarda impressão dos Bs
     printf("C"); 
     pthread_barrier_wait(&barrier); //sinaliza que imprimiu C
     pthread_barrier_wait(&barrier); //aguarda impressão dos Ds

     pthread_barrier_wait(&barrier); //aguarda impressão da nova linha
  }
}



void * func_pbd(void *arg) {
  int myid = *(int *)(arg);
  while(1){
    pthread_barrier_wait(&barrier); //aguarda impressão dos As
    printf("B");
    pthread_barrier_wait(&barrier); //sinaliza que imprimiu B
    pthread_barrier_wait(&barrier); //aguarda impressão dos Cs
    printf("D");
    pthread_barrier_wait(&barrier); //sinaliza que imprimiu D
    
    if(myid == 0){
        printf("\n");
        sleep(5);
    }
    pthread_barrier_wait(&barrier); //sinaliza que imprimiu a nova linha 
  }
}


int main(int argc, char *argv[]) {
  
  pthread_t pacId[PAC];
  pthread_t pbdId[PBD];

  pthread_barrier_init(&barrier, NULL, PAC+PBD);


  int * id;
  int i;
  for (i = 0; i < PAC; i++){
     id = (int *) malloc(sizeof(int));
     *id = i;
     pthread_create(&pacId[i], NULL, func_pac, (void *) (id));

  }
  for (i = 0; i < PBD; i++){
     id = (int *) malloc(sizeof(int));
     *id = i;
     pthread_create(&pbdId[i], NULL, func_pbd, (void *) (id));

  }


  for (i = 0; i < PAC; i++){
       if(pthread_join(pacId[i], NULL)){
          printf("\n ERROR joining thread");
          exit(1);
       }
  }


  for (i = 0; i < PBD; i++){
       if(pthread_join(pbdId[i], NULL)){
          printf("\n ERROR joining thread");
          exit(1);
       }
  }

  printf("\nBye!\n");
}


