#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include "utils.h"
#include "queue.h"


// global variable for clock cycle
int tick = 0;


/**
 * A function that simulates the Round Robin pre-emptive scheduling policy.
 * Each process is run with a single clock cycle (sleep of 1 second) 
 * At each clock cycle:
 *      - the current process is checked with its remaining service time and quantum time whether it has finished executing or not
 *        and is only allowed to run within the quantum time slice until clock interrupt occurs
 *      - the process queue is checked for arriving processes to be inserted into the ready queue
 *      - the ready queue is served with the next process when the current executing process is finished 
 *      - if there are any more processes to be served and executed
 * 
 * Arguments: 
 *      processes - the array of sorted processes (by arrival time)
 *      length - the number of processes
 *      processQueue - a Queue struct that stores all processes from the processes array
 *      readyQueue - a Queue struct that stores processes that are ready to be executed
 *      quantum - the time slice
 * 
 * Returns: none
 **/
void runRoundRobin(pcb_t *processes, int length, Queue processQueue, Queue readyQueue, int quantum) {
    // boolean value to check if the CPU is currently occupying and executing by a process
    bool occupied = false;
    // boolean value to ensure that 'running state' messages are only printed once
    bool printed = false;

    // insert the sorted processes into the process queue
    for(int i = 0; i < length; i++) {
        processes[i].quantumTime = quantum;
        insertQueue(&processQueue, &processes[i]);
    }

    /** pointers that specifies:
     * next_process = the next process to be inserted into the ready queue
     * current_process = the current process being executed by the CPU
     **/
    pcb_t *next_process, *current_process = NULL;

    // an infinite loop that runs continiously until there is no more process in ready/process queue and CPU is not occupied
    while(true) {        
        // check for any new processes with its arrival time matching the current clock cycle
        while(queueEmpty(&processQueue) == false && checkHead(&processQueue)->entryTime == tick) {
            next_process = serveQueue(&processQueue);
            next_process->state = START;
        
            insertQueue(&readyQueue, next_process);
            if(current_process == NULL) {
                current_process = serveQueue(&readyQueue);
                printed = false;
                occupied = true;
            }

            printf("Time %d: %s has entered the system.\n", tick, next_process->process_name);          
        } 

        // means that the current process has finished its time slice and went back to ready queue
        if(queueEmpty(&readyQueue) == false && (current_process->quantumTime == 0 || occupied == false || current_process->state == EXIT)) {
            // reset the time quantum for that process so later it can run again
            current_process->quantumTime = quantum;
            // replace the current process with the next process in ready queue
            current_process = serveQueue(&readyQueue);
            occupied = true;
            printed = false;
        } 

        // if there are no processes in the ready queue and CPU
        if(queueEmpty(&readyQueue) && occupied == false) {
            printf("Time %d: CPU is idle.\n", tick);
            sleep(1);
            tick = tick + 1;
            continue;
        }

        current_process->state = RUNNING;
        occupied = true;
        
        if(current_process->state == RUNNING && printed == false) {
            printf("Time %d: %s is in the running state.\n", tick, current_process->process_name);
            printed = true;
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

            /**
             * Calculation of scheduler results:
             * 1. turnaround time = finish time - entry time
             * 2. wait time = turnaround time - service time
             **/
            current_process->turnaroundTime = tick - current_process->entryTime;
            current_process->waitTime = current_process->turnaroundTime - current_process->serviceTime;

            if(tick - current_process->entryTime <= current_process->deadlineTime) {
                current_process->deadlineMet = 1;
            } else {
                current_process->deadlineMet = 0;
            }
        } else {
            // if the process has used up its time slice
            // clock interrupt occurs
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
                while(queueEmpty(&processQueue) == false && checkHead(&processQueue)->entryTime == tick) {
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

        // if there are no more processes in ready and process queue and CPU is not occupied
        // then the scheduler has finished
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
    char *filename;
    if(argc == 1) {
        filename = "process-data.txt";
    } else if(argc > 2) {
        printf("Format : ./task2 [filename]\n");
        exit(1);
    } else {
        filename = argv[1];
    }

    int length = count_processes(filename)+1;
    // insert the processes in the file to pcb_t array
    pcb_t *processes = get_pcb(filename);
    Queue processQueue, readyQueue;

    processQueue = initializeQueue(length);
    readyQueue = initializeQueue(length*2);

    runRoundRobin(processes, length, processQueue, readyQueue, 2);
    
    return 0;
}