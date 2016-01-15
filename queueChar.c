#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

struct queue{
    char **v;
    int head;
    int tail;
    int size;
    int maxsize;
};
struct queue *queue_create(int maxsize)
{
    struct queue *q = malloc(sizeof(*q));
		q->v = (char**)malloc(sizeof(char*) * maxsize);
    if (q != NULL){
				int i;
				for(i = 0; i < maxsize + 1; i++){
					q->v[i] = (char*)malloc(sizeof(char)*500);
				}
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
int queue_enqueue(struct queue *q, char* value){
    if (q->head == q->tail + 1)
        return -1;
    q->v[q->tail++] = value;
    q->tail = q->tail % (q->maxsize + 1);
    q->size++;
    return 0;
}
char* queue_dequeue(struct queue *q){
    if (q->head % (q->maxsize + 1) == q->tail){
        return "queue is empty";
    }
    q->head = q->head % (q->maxsize + 1);
    q->size--;
		return q->v[q->head++];
}
