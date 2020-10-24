#ifndef QUEUE
#define QUEUE

#include <stdbool.h>
#include "process.h"

typedef struct {
    pcb_t **array;
    int size;
    int head;
    int tail;
} Queue;

Queue initializeQueue(int n);
bool insertQueue(Queue *queue, pcb_t *pcb);
bool queueEmpty(Queue *queue);
pcb_t *serveQueue(Queue *queue);
pcb_t *checkHead(Queue *queue);

#endif

