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



void runRoundRobin(pcb_t *processes, int length, Queue processQueue, Queue readyQueue, int quantum) {
    bool occupied = false;

    for(int i = 0; i < length; i++) {
        processes[i].quantumTime = quantum;
        insertQueue(&processQueue, &processes[i]);
    }

    pcb_t *next_process, *current_process = NULL;

    while(true) {
        // if(queueEmpty(&readyQueue)) {
        //     printf("Time %d : CPU is idle.", tick);
        //     sleep(1);
        //     continue;
        // }
        
        // check for any new processes
        if(queueEmpty(&processQueue) == false && checkHead(&processQueue)->entryTime == tick) {
            next_process = serveQueue(&processQueue);
            next_process->state = START;
        
            insertQueue(&readyQueue, next_process);
            if(current_process == NULL) {
                current_process = serveQueue(&readyQueue);
            }

            printf("Time %d: %s has entered the system.\n", tick, next_process->process_name);          
        } 

        // means that the current process has finished its time slice and went back to ready queue
        if(queueEmpty(&readyQueue) == false && (occupied == false || current_process->quantumTime == 0 || current_process->state == EXIT)) {
            current_process->quantumTime = quantum;
            // replace the current process with the next process in ready queue
            current_process = serveQueue(&readyQueue);
        } 

        current_process->state = RUNNING;
        occupied = true;
        
        if(current_process->state == RUNNING) {
            printf("Time %d: %s is in the running state.\n", tick, current_process->process_name);
        }

        sleep(1);
        tick = tick + 1;
        current_process->quantumTime = current_process->quantumTime - 1;
        current_process->remainingTime = current_process->remainingTime - 1;

        // means that the process has finished its total execution
        if(current_process->remainingTime <= 0) {
            occupied = false;
            current_process->state = EXIT;
            if(current_process->state == EXIT) {
                printf("Time %d: %s has finished execution.\n", tick, current_process->process_name);
            }

            // the total time the process spends in the system
            // current_process->turnaroundTime = current_process->waitTime + current_process->serviceTime;
            current_process->turnaroundTime = tick - current_process->entryTime;
            current_process->waitTime = current_process->turnaroundTime - current_process->serviceTime;

            if(tick - current_process->entryTime <= current_process->deadlineTime) {
                current_process->deadlineMet = 1;
            } else {
                current_process->deadlineMet = 0;
            }
        } else {
            if(current_process->quantumTime == 0) {
                // process goes back into ready queue
                printf("Time %d: %s has run out of time slice.\n", tick, current_process->process_name);
                occupied = false;


                /**
                 * Assumption starts here:
                 * 1. We prioritize newly-entered processes (if available) over just-ready processes
                 * 2. Therefore newly-entered processes would be inserted into the ready queue first over the just-ready processes
                 * 
                 * Reason : New processes do not have a chance to run yet so we prioritize them first
                 * Note : Assumption can be changed (for marking purposes) by just switching the order of statement 1 and statement 2
                 **/

                // Statement 1 : Prioritized and insert the newly entered process first (if available) to the queue over quantum-finished process
                if(queueEmpty(&processQueue) == false && checkHead(&processQueue)->entryTime == tick) {
                    next_process = serveQueue(&processQueue);
                    next_process->state = START;
        
                    insertQueue(&readyQueue, next_process);
                    printf("Time %d: %s has entered the system.\n", tick, next_process->process_name); 
                }

                // statement 2 : Insert the just-ready processes next
                insertQueue(&readyQueue, current_process);

                /**
                 * Assumption ends here
                 **/
            } else {
                // the process should continue running since it still has enough time slice 
                // and thus the CPU will still be occupied by that process
                occupied = true;
            }
        }

        if(queueEmpty(&processQueue) && queueEmpty(&readyQueue) && occupied == false) {
            printf("All processes have finished execution.\n");
            log_result(processes, length);
            break;
        }
    }
    // free up any allocated memory
    free(processes);
    free(processQueue.array);
    free(readyQueue.array);
}

int main(int argc, char **argv) {
    if(argc == 1) {
        printf("Provide file name!\n");
        exit(1);
    } else if(argc > 2) {
        printf("Format : ./task2 [filename]\n");
        exit(1);
    }

    char *filename = argv[1];

    int length = count_processes(filename)+1;
    // insert the processes in the file to pcb_t array
    pcb_t *processes = get_pcb(filename);

    Queue processQueue, readyQueue;

    processQueue = initializeQueue(length);
    readyQueue = initializeQueue(length*2);

    runRoundRobin(processes, length, processQueue, readyQueue, 2);
    
    return 0;
}