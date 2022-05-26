#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include<sys/syscall.h>
#include <unistd.h>
/*
This program provides a possible solution for producer-consumer problem using mutex and semaphore.
*/

#define MaxItems 3 // Maximum items a producer can produce or a consumer can consume
#define BufferSize 3 // Size of the buffer
#define MaxProducers 5 // Maximum number of Producers
#define MaxConsumers 5 // Maximum number of Consumers

long int s=0;
sem_t empty;
sem_t full;
int in = 0;
int out = 0;
int buffer[BufferSize];
pthread_mutex_t mutex;

void *producer(void *pno)
{   
    int item;
    for(int i = 0; i < MaxItems; i++) {
        item = rand() % 1000; // Produce a random item
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[in] = item;
	s=syscall(335,2,*((int*)pno),in,buffer[in]);
	printf("\nthe syscall returned %ld",s);
	//printf("Producer %d: Produces an Item %d at buffer loc %d\n", *((int *)pno),buffer[in],in);
	sleep(1); // Assume that it takes overall one second to produce an item
        in = (in+1)%BufferSize;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
    printf("Producer %d has completed producing\n", *((int *)pno));
}
void *consumer(void *cno)
{   
    for(int i = 0; i < MaxItems; i++) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        int item = buffer[out];
	s=syscall(335,1,*((int*)cno),out,item);
	//printf("Consumer %d: Consumes an Item %d from buffer loc %d\n",*((int *)cno),item, out);
        printf("\nthe syscall returned %ld",s);
	out = (out+1)%BufferSize;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
    
    printf("Consumer %d has completed consuming\n", *((int *)cno));
}

int main()
{   

    pthread_t pro[5],con[5];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty,0,BufferSize);
    sem_init(&full,0,0);

    int a[5] = {1,2,3,4,5}; //Just used for numbering the producer and consumer

	printf("\t|||Producer Consumer Problem Solution Using Semaphores|||\n");
	
	printf("\n\tTotal number of producers are: %d", MaxProducers);
	printf("\n\tTotal number of consumers are: %d", MaxConsumers);
	printf("\n\tThe number of max items produced or consumed are : %d", MaxItems);
	printf("\n\tBuffer size for items is : %d\n\n", BufferSize);

    for(int i = 0; i < 5; i++) {
        pthread_create(&pro[i], NULL, (void *)producer, (void *)&a[i]);
    }
    for(int i = 0; i < 5; i++) {
        pthread_create(&con[i], NULL, (void *)consumer, (void *)&a[i]);
    }

    for(int i = 0; i < 5; i++) {
        pthread_join(pro[i], NULL);
    }
    for(int i = 0; i < 5; i++) {
        pthread_join(con[i], NULL);
    }

	printf("\n\n\tEvery Producer has produced %d items", MaxItems);
	printf("\n\tEvery Consumer has consumed %d items\n", MaxItems);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
    
}
