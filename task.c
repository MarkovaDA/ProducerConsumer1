#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
 
struct queue{
    int *v;
    int head;
    int tail;
    int size;
    int maxsize;
};
struct queue *queue_create(int maxsize)
{
    struct queue *q = malloc(sizeof(*q));
    if (q != NULL){
        q->v = malloc(sizeof(int)*(maxsize + 1));
        if (q->v == NULL){
            free(q); return NULL;
        }
        q->maxsize = maxsize;
        q->size = 0;
        q->head = maxsize + 1;
        q->tail = 0;
    }
    return q;
}
void queue_free(struct queue *q){
    free(q->v);
    free(q);
}
int queue_size(struct queue *q)
{
    return q->size;
}
int queue_enqueue(struct queue *q, int value){
    if (q->head == q->tail + 1)
        return -1;
    q->v[q->tail++] = value;
    q->tail = q->tail % (q->maxsize + 1);
    q->size++;
    return 0;
}
int queue_dequeue(struct queue *q){
    if (q->head % (q->maxsize + 1) == q->tail){
        return -1;
    }
    q->head = q->head % (q->maxsize + 1);
    q->size--;
		return q->v[q->head++];
}


static void *producer(void* arg);

struct queue *queueSource;//буфер для хранения промежуточных результатов
pthread_mutex_t* myMutex;//мьютекс для синхронизации доступа к очереди
pthread_cond_t* onPutCondition; //событие,которое будет активироваться при добавлении в очередь данных
FILE *f;//исходный файл с чилами
int totalSum; //сумма всех чисел в файле

int main(){
		f = fopen("source.txt", "r");
		if (f == NULL)
			exit(EXIT_FAILURE);
    queueSource = queue_create(100);
		pthread_t prod;//производитель
		//инициализация потока производителя
		int resCreate =  pthread_create(&prod, NULL, producer, NULL);
    if(resCreate != 0){
            printf("Error creating producer-thread\n");
            exit(1);
    }
    resCreate = pthread_join(prod, NULL);
    if(resCreate != 0)
    {
        printf("Thread producer joining failed\n");
        exit(1);
    }
		fclose(f);
		printf("%s", "производитель закончил работу");
    return 0;   
}

/*
*функция производителя: читает файл строка за строкой
*в каждой строке выделяет числа, суммирует их
*и кладет результат в общий буфер - queueSource
*/
static void *producer(void* arg)
{
    char *line = NULL; int temp;
		size_t len = 0;
		ssize_t read;
    while ((read = getline(&line, &len, f)) != -1) {
          char *token;
          char *rest = line;
          int sumIntoStr = 0;
          token = strtok(rest, " ");
					while(token != NULL){
							temp = atoi(token);
							sumIntoStr += temp;
							token = strtok(NULL, " ");
					}
          //кладём сумму строки в очередь
          queue_enqueue(queueSource, sumIntoStr);
    }
}
