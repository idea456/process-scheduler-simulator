#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include "utils.h"
#include "queue.h"


// global variable for clock
int tick = 0;
// global variable for time termination
bool end = false;

// struct for encapsulating thread arguments
typedef struct {
    Queue *processQueue;
    Queue *readyQueue;
} Params;

// the thread function to be continiously executed parallely
void *threadFunction(void *context) {
    Params *params = (Params *)context;

    while(end == false) {
        if(queueEmpty(params->processQueue) == false && checkHead(params->processQueue)->entryTime == tick) {
            pcb_t *next_process = serveQueue(params->processQueue);
            next_process->state = START;
            insertQueue(params->readyQueue, next_process);
            if(next_process->state == START) {
                printf("Time %d: %s has entered the system.\n", tick, next_process->process_name);
            }     
        } 
        tick = tick + 1;
        sleep(1);
    }
    return 0;
}

void runFCFS(pcb_t *processes, int length, Queue processQueue, Queue readyQueue) {
    for(int i = 0; i < length; i++) {
        insertQueue(&processQueue, &processes[i]);
    }

    pcb_t *next_process;
    pcb_t *ready_process;
    pthread_t tid;
    Params params;

    ready_process = serveQueue(&processQueue);
    ready_process->state = START;
    insertQueue(&readyQueue, ready_process);
    printf("Time %d: %s has entered the system.\n", tick, ready_process->process_name);

    params.processQueue = &processQueue;
    params.readyQueue = &readyQueue;

    /** create a new clock thread that performs :
     * 1. Incrementation of clock cycle
     * 2. Place arriving processes into ready queue at specific clock cycle
     **/
    pthread_create(&tid, NULL, threadFunction, &params);

    while(queueEmpty(&readyQueue) == false || queueEmpty(&processQueue) == false) {
        // if there are no more ready processes but there are still available processes 
        // then processor is idle, waiting for new processes to come in 
        if(queueEmpty(&readyQueue)) {
            printf("Time %d : Processor is idle.\n", tick);
            sleep(1);
            continue;
        }
        
        next_process = serveQueue(&readyQueue);

        next_process->waitTime = tick - next_process->entryTime;

        next_process->state = RUNNING;
        if(next_process->state == RUNNING) {
            printf("Time %d: %s is in the running state.\n", tick, next_process->process_name);
        }
        sleep(next_process->serviceTime);



        next_process->state = EXIT;
        if(next_process->state == EXIT) {
            printf("Time %d: %s has finished execution.\n", tick, next_process->process_name);
        }
        next_process->turnaroundTime = tick - next_process->entryTime;
        if(tick -next_process->entryTime <= next_process->deadlineTime) {
            next_process->deadlineMet = 1;
        } else {
            next_process->deadlineMet = 0;
        }

        if(queueEmpty(&processQueue) && queueEmpty(&readyQueue)) {
            end = true;
            log_result(processes, length);
            break;
        }
    }

    // terminate the clock thread
    pthread_join(tid, NULL);
    // free up any allocated memory
    free(processes);
    free(processQueue.array);
    free(readyQueue.array);
}

int main() {
    int length = count_processes("process-data.txt")+1;
    // insert the processes in the file to pcb_t array
    pcb_t *processes = get_pcb("process-data.txt");

    Queue processQueue, readyQueue;

    processQueue = initializeQueue(length);
    readyQueue = initializeQueue(length*2);

    runFCFS(processes, length, processQueue, readyQueue);
    
    return 0;
}