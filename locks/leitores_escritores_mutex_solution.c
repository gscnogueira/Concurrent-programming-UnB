#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "pthread.h"

#define TRUE 1


#define NE 10 //numero de escritores
#define NL 20 //numero de leitores


pthread_mutex_t turno; 
pthread_mutex_t mutex;                    /* controla o acesso a 'rc' */
pthread_mutex_t db;                       /* controla o acesso a base de dados */
int rc = 0;                     /* número de processos lendo ou querendo ler */

void * reader(void *arg);
void * writer(void *arg);
void read_data_base();
void use_data_read();
void think_up_data();
void write_data_base();

int main() {
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&db, NULL);
	pthread_mutex_init(&turno, NULL);

	pthread_t r[NL], w[NE];

	int i;
        int *id;
        /* criando leitores */
    	for (i = 0; i < NL ; i++) {
	   id = (int *) malloc(sizeof(int));
           *id = i;
		 pthread_create(&r[i], NULL, reader, (void *) (id));
	}

	 /* criando escritores */
	for (i = 0; i< NE; i++) {
	   id = (int *) malloc(sizeof(int));
           *id = i;
		 pthread_create(&w[i], NULL, writer, (void *) (id));
	}

	pthread_join(r[0],NULL);
	return 0;
}

void* reader(void *arg) {
	int i = *((int *) arg);
	while(TRUE) {               /* repere para sempre */
		 pthread_mutex_lock(&turno); 
		 pthread_mutex_lock(&mutex);    
		 rc = rc + 1;            
		 if (rc == 1) {          
		     pthread_mutex_lock(&db);   
		 }
		 pthread_mutex_unlock(&mutex);
	         pthread_mutex_unlock(&turno); 
		 read_data_base(i);       
		 pthread_mutex_lock(&mutex);
		 rc = rc - 1;            
		 if (rc == 0) {          
		     pthread_mutex_unlock(&db);
		 }
		 pthread_mutex_unlock(&mutex);
		 use_data_read(i);        /* região não crítica */
	}
        pthread_exit(0);
}

void* writer(void *arg) {
	int i = *((int *) arg);

	while(TRUE) {               /* repete para sempre */
		 think_up_data(i);        /* região não crítica */
		 pthread_mutex_lock(&turno);
		 pthread_mutex_lock(&db);          /* obtém acesso exclusivo */
		 write_data_base(i);      /* atualiza os dados */
		 pthread_mutex_unlock(&db);          /* libera o acesso exclusivo */
		 pthread_mutex_unlock(&turno);
	}
        pthread_exit(0);
}

void read_data_base(int i) {
	printf("Leitor %d está lendo os dados! Número de leitores = %d \n", i,rc);
	sleep( rand() % 5);
}

void use_data_read(int i) {
	printf("Leitor %d está usando os dados lidos!\n", i);
	sleep(rand() % 5);
}

void think_up_data(int i) {
	printf("Escritor %d está pensando no que escrever!\n", i);
	sleep(rand() % 5);
}

void write_data_base(int i) {
	printf("Escritor %d está escrevendo os dados!\n", i);
	sleep( rand() % 5 + 5);
}
