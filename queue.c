#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include "process.h"
#include "queue.h"

/**
 * A function that initializes an empty Queue with the appropriate values
 * 
 * Arguments:
 *      n - the size of the queue to initialize
 * Returns : An initialized Queue
 **/
Queue initializeQueue(int n) {
    Queue new_queue;
    // allocate memory for pcb_t pointers
    new_queue.array = malloc(sizeof(pcb_t) * n);
    // initialize all the appropriate values
    new_queue.size = 0;
    new_queue.head = -1;
    new_queue.tail = -1;
    return new_queue;
}

/**
 * A function that indicates if the argument Queue is empty or not
 * Arguments:
 *      queue - the Queue struct to check if it empty or not
 * Returns : A boolean value indicating whether the Queue is empty or not
 **/
bool queueEmpty(Queue *queue) {
    if(queue->size == 0) {
        return true;
    } 
    return false;
}

/**
 * A function which looks at the contents of the head of the argument queue
 * Arguments:
 *      queue - the Queue struct to look into
 * Returns : The process which was at the head of the argument queue
 **/
pcb_t *checkHead(Queue *queue) {
    return queue->array[queue->head];
}

/**
 * A function that inserts a process into the provided argument Queue
 * Arguments:
 *      queue - the Queue struct to insert the process into
 *      pcb - the process to be inserted into
 * Returns : A boolean value indicating whether the insertion is successful or not
 **/
bool insertQueue(Queue *queue, pcb_t *pcb) {
    if(queue->head == -1) {
        queue->head = 0;
    }
    queue->tail = queue->tail + 1;
    queue->array[queue->tail] = pcb;
    queue->size = queue->size + 1;
    return true;
}

/**
 * A function that serves a process from the argument Queue head
 * Arguments:
 *      queue - the Queue struct to serve to
 * Returns : The process which was initially at the head of the argument queue and was served from the queue
 **/
pcb_t *serveQueue(Queue *queue) {
    pcb_t *process;

    // get the process at the head index
    process = queue->array[queue->head];
    queue->head = queue->head + 1;
    queue->size = queue->size - 1;
    return process;
}

