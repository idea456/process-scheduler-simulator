#include <stdio.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "process.h"

/**
 * A function that converts an integer into a string using sprintf
 * 
 * Arguments:
 *      n - the integer to convert into string
 *      str - the string pointer
 * Returns : The string pointer containing the converted string integer
 **/
char *to_string(int n, char *str) {
    sprintf(str, "%d", n);
    return str;
}

/**
 * A function that counts the number of processes in the text file
 * 
 * Arguments:
 *      filename - the name of the text file which contains the list of processes with their respective values
 * Returns : The number of processes in the text file
 **/
int count_processes(char *filename) {
    int processes = 0;
    int infile;
    char buffer[1024];
    char s;

    // if file does not exist
    if((infile = open(filename, O_RDONLY, 0664)) < 0) {
        // return error message to standard output error stream
        perror(filename);
        // exit with return value of 1
        exit(1);
    }

    while(s = read(infile, buffer, 1) != 0) {
        if(strncmp("\n", buffer, 1) == 0) {
            processes = processes + 1;
        }
    }
    close(infile);
    return processes;
}


/**
 * A function that sorts the array of pcb_t processes based on their arrival time in ascending order
 * The sorting algorithm used is called Insertion Sort
 * 
 * Arguments:
 *      processes - the array of pcb_t processes to sort
 * Returns : The sorted array of pcb_t processes based on their arrival time
 **/
pcb_t *sort_pcb(pcb_t *processes, int n) {
    pcb_t key;
    int j;
    for(int i = 1; i < n; i++) {
        key = processes[i];
        j = i - 1;
        while(j >= 0 && processes[j].entryTime> key.entryTime) {
            processes[j + 1] = processes[j];
            j = j - 1;
        }
        processes[j + 1] = key;
    }
    return processes;
}

/**
 * A function that looks into the list of processes and their respective values in the text file
 * and formats them into a pcb_t struct, and inserts all of them into an array of pcb_t
 * 
 * Arguments:
 *      filename - the name of the text file which contains the list of processes with their respective values
 * Returns : An array of pcb_t processes formatted from the text file
 **/
pcb_t *insert_pcb(char *filename) {
    int count = count_processes(filename) + 1;
    pcb_t *pcbs = malloc(sizeof(pcb_t) * count);

    FILE* infile = fopen(filename, "r");
    char line[100];
    char delimiter[] = " ";

    char *token;
    int i = 0;

    while(fgets(line, sizeof(line), infile)) {
        token = strtok(line, delimiter);
        pcb_t pcb;

        strncpy(pcb.process_name, token, 11);
        token = strtok(NULL, delimiter);
        pcb.entryTime = atoi(token);
        token = strtok(NULL, delimiter);
        pcb.serviceTime = atoi(token);
        token = strtok(NULL, delimiter);
        pcb.deadlineTime = atoi(token);

        pcb.remainingTime = pcb.serviceTime;
        pcb.hasEntered = false;
        pcb.waitTime = 0;

        pcbs[i] = pcb;
        i = i + 1;

    }

    fclose(infile);
    return pcbs;
}

/**
 * A function that encapsulates the steps of formatting and sorting the list of processes from the text file
 * into a sorted array of pcb_t processes
 * 
 * Arguments:
 *      filename - the text file containing the list of processes
 * Returns : The sorted array of pcb_t processes
 **/
pcb_t *get_pcb(char *filename) {
    int count = count_processes(filename) + 1;
    pcb_t *processes = insert_pcb(filename);
    return sort_pcb(processes, count);
}

/**
 * A function that logs the results of processes such as wait time, turnaround time, and whether the deadline is met
 * into a text file called scheduler-result.txt
 * 
 * Arguments:
 *      processes - the array of processes
 *      n - the number of processes in the processes array
 * Returns : none
 **/
void log_result(pcb_t *processes, int n) {
    FILE *ptr;
    char waitTime[10], turnaroundTime[10], deadlineMet[1];

    ptr = fopen("scheduler-result.txt", "w");

    for(int i = 0; i < n; i++) {
        fputs(processes[i].process_name, ptr);
        fputs(" ", ptr);
        to_string(processes[i].waitTime, waitTime);
        fputs(waitTime, ptr);
        fputs(" ", ptr);
        to_string(processes[i].turnaroundTime, turnaroundTime);
        fputs(turnaroundTime, ptr);
        fputs(" ", ptr);
        to_string(processes[i].deadlineMet, deadlineMet);
        fputs(deadlineMet, ptr);
        fputs("\n", ptr);
    } 

    // close the file
    fclose(ptr);
}