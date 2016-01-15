
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include "queueChar.h"

#define WORKERS 2

pthread_mutex_t mainMutex, onEndMutex;
pthread_cond_t onPutCondition, onTakeCondition;
FILE *f;	
int out = 0, in = 0;
struct queue *queueSource;//буфер для хранения промежуточных результатов
int complete = 0, totalSum = 0;

void* producer(void *ptr) {
 int i;	
 char *line = NULL; int temp;
 size_t len = 0;
 ssize_t read;
 while ((read = getline(&line, &len, f)) != -1) {		
  pthread_mutex_lock(&mainMutex);	
  //кладем в буфер прочитанную строчку
  queue_enqueue(queueSource, line); in++;		
  printf("in producer put into queue: %s\n", line);
  //если очередь была пуста, пробуждаем потребителя 
  if (queue_size(queueSource) - 1 == 0)
  pthread_cond_signal(&onPutCondition);	
  pthread_mutex_unlock(&mainMutex);	 
  sleep(1);
}
 complete = 1;
 pthread_exit(0);
}

void* consumer(void *ptr) {
int i;
while (1) {
 //попытка остановить работу процессов в нужный момент
 pthread_mutex_lock(&mainMutex);
 if (complete != 0){
   pthread_mutex_lock(&onEndMutex);
	 if (complete != 0 && out == in)
    pthread_exit(0);
   pthread_mutex_unlock(&onEndMutex);
 }	
 //Буфер пуст - ждём пополнения
 while(queue_size(queueSource) == 0)
  pthread_cond_wait(&onPutCondition, &mainMutex);
 //берём из буфера строчку
 char *rest = queue_dequeue(queueSource); out++;
 printf("in consumer take from queue: %s\n", rest);
 //считаем её сумму и приплюсовываем к totalSum
 char *token;
 int sumIntoStr = 0;
 token = strtok(rest, " ");
 while(token != NULL){
  int temp = atoi(token);
  sumIntoStr += temp;
  token = strtok(NULL, " ");
}
 totalSum += sumIntoStr;
 printf("in consumer sum: %d\n", totalSum);
 pthread_mutex_unlock(&mainMutex);	
}
pthread_exit(0);
}

int main(int argc, char **argv) {
 f = fopen("source.txt", "r");
 if (f == NULL)
  exit(EXIT_FAILURE);
 pthread_t pro, con[WORKERS];
 queueSource = queue_create(10);
 pthread_mutex_init(&mainMutex, NULL);
 pthread_mutex_init(&onEndMutex, NULL);	
 pthread_cond_init(&onPutCondition, NULL);		
 pthread_cond_init(&onTakeCondition, NULL);		
 int i;
 for(i = 0; i < WORKERS; i++){
  pthread_create(&con[i], NULL, consumer, NULL);
 }
 pthread_create(&pro, NULL, producer, NULL);
 pthread_join(pro, NULL);
 //пока для 2 потребителей
 pthread_join(con[0], NULL);
 pthread_join(con[1], NULL);
 pthread_mutex_destroy(&mainMutex);	
 pthread_cond_destroy(&onPutCondition);		
 pthread_cond_destroy(&onTakeCondition);		
}
