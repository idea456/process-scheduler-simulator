#ifndef UTILS
#define UTILS

#include "process.h"

/**
 * A function that counts the number of processes in the text file
 * 
 * Arguments:
 *      filename - the name of the text file which contains the list of processes with their respective values
 * Returns : The number of processes in the text file
 **/
int count_processes(char *filename);
/**
 * A function that looks into the list of processes and their respective values in the text file
 * and formats them into a pcb_t struct, and inserts all of them into an array of pcb_t
 * 
 * Arguments:
 *      filename - the name of the text file which contains the list of processes with their respective values
 * Returns : An array of pcb_t processes formatted from the text file
 **/
pcb_t *insert_pcb(char *filename);
/**
 * A function that sorts the array of pcb_t processes based on their arrival time in ascending order
 * The sorting algorithm used is called Insertion Sort
 * 
 * Arguments:
 *      processes - the array of pcb_t processes to sort
 * Returns : The sorted array of pcb_t processes based on their arrival time
 **/
pcb_t *sort_pcb(pcb_t *processes);
/**
 * A function that encapsulates the steps of formatting and sorting the list of processes from the text file
 * into a sorted array of pcb_t processes
 * 
 * Arguments:
 *      filename - the text file containing the list of processes
 * Returns : The sorted array of pcb_t processes
 **/
pcb_t *get_pcb(char *filename);
/**
 * A function that logs the results of processes such as wait time, turnaround time, and whether the deadline is met
 * into a text file called scheduler-result.txt
 * 
 * Arguments:
 *      processes - the array of processes
 *      n - the number of processes in the processes array
 * Returns : none
 **/
void log_result(pcb_t *processes, int n);

#endif
