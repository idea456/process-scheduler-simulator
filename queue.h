#ifndef QUEUE
#define QUEUE

#include <stdbool.h>
#include "process.h"

/**
 * A struct that is used to emulate a queue data structure
 * array - the array to hold pointers to pcb_t objects
 * size - integer containing the size of the queue
 * head - the index which always points to the head of the queue (start of the array)
 * tail - the index which always points to the tail of the queue (end of the array)
 **/
typedef struct {
    pcb_t **array;
    int size;
    int head;
    int tail;
} Queue;

/**
 * A function that initializes an empty Queue with the appropriate values
 * 
 * Arguments:
 *      n - the size of the queue to initialize
 * Returns : An initialized Queue
 **/
Queue initializeQueue(int n);
/**
 * A function that inserts a process into the provided argument Queue
 * Arguments:
 *      queue - the Queue struct to insert the process into
 *      pcb - the process to be inserted into
 * Returns : A boolean value indicating whether the insertion is successful or not
 **/
bool insertQueue(Queue *queue, pcb_t *pcb);
/**
 * A function that indicates if the argument Queue is empty or not
 * Arguments:
 *      queue - the Queue struct to check if it empty or not
 * Returns : A boolean value indicating whether the Queue is empty or not
 **/
bool queueEmpty(Queue *queue);
/**
 * A function that serves a process from the argument Queue head
 * Arguments:
 *      queue - the Queue struct to serve to
 * Returns : The process which was initially at the head of the argument queue and was served from the queue
 **/
pcb_t *serveQueue(Queue *queue);
/**
 * A function which looks at the contents of the head of the argument queue
 * Arguments:
 *      queue - the Queue struct to look into
 * Returns : The process which was at the head of the argument queue
 **/
pcb_t *checkHead(Queue *queue);

#endif

