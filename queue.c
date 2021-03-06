#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
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
int main(){
		struct queue *q;
    int i,val;
    q = queue_create(10);
    val = queue_dequeue(q); 
    queue_enqueue(q, 0);
    queue_enqueue(q, 5);
    queue_enqueue(q, 2);
    queue_enqueue(q, 3);
    queue_enqueue(q, 12);
    queue_enqueue(q, 4);
    printf("%d\n", queue_size(q)); 
    return 0;   
}
