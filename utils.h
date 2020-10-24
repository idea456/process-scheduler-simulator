#ifndef UTILS
#define UTILS

#include "process.h"

int count_processes(char *filename);
pcb_t *insert_pcb(char *filename);
pcb_t *sort_pcb(pcb_t *processes);
pcb_t *get_pcb(char *filename);
void log_result(pcb_t *processes, int n);
#endif
