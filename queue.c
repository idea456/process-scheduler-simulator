#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include "process.h"
#include "queue.h"


Queue initializeQueue(int n) {
    Queue new_queue;
    new_queue.array = malloc(sizeof(pcb_t) * n);
    new_queue.size = 0;
    new_queue.head = -1;
    new_queue.tail = -1;
    return new_queue;
}

bool queueEmpty(Queue *queue) {
    if(queue->size == 0) {
        return true;
    } 
    return false;
}

pcb_t *checkHead(Queue *queue) {
    return queue->array[queue->head];
}

bool insertQueue(Queue *queue, pcb_t *pcb) {
    if(queue->head == -1) {
        queue->head = 0;
    }
    queue->tail = queue->tail + 1;
    queue->array[queue->tail] = pcb;
    queue->size = queue->size + 1;
    return true;
}

pcb_t *serveQueue(Queue *queue) {
    pcb_t *process;

    process = queue->array[queue->head];
    queue->head = queue->head + 1;
    queue->size = queue->size - 1;
    return process;
}

