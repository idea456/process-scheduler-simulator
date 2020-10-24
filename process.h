#include <stdbool.h>

#ifndef PROCESS
#define PROCESS

typedef enum {
    START, RUNNING, WAITING, EXIT
} process_state_t;


typedef struct {
    char process_name[11];
    int entryTime;
    int serviceTime;
    int remainingTime;
    int quantumTime;
    int deadlineTime;
    int waitTime;
    int turnaroundTime;
    int deadlineMet;
    bool hasEntered;
    process_state_t state;
} pcb_t;
 
#endif