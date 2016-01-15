struct queue *queue_create(int maxsize);
void queue_free(struct queue *q);
int queue_size(struct queue *q);
int queue_enqueue(struct queue *q, char* value);
char* queue_dequeue(struct queue *q);
