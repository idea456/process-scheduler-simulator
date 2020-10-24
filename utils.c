#include <stdio.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "process.h"

char *to_string(int n, char *str) {
    sprintf(str, "%d", n);
    return str;
}

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


// since we cannot assume that the process will arrive by their arrival time
pcb_t *sort_pcb(pcb_t *processes, int n) {
    // use insertion sort algorithm from GeeksforGeeks
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

pcb_t *insert_pcb(char *filename) {
    int count = count_processes(filename) + 1;
    pcb_t *pcbs = malloc(sizeof(pcb_t) * count);

    FILE* infile = fopen(filename, "r");
    char line[100];
    char delimiter[] = " ";

    char *token;
    int i = 0;

    while(fgets(line, sizeof(line), infile)) {
        // printf("%s\n", line);
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

pcb_t *get_pcb(char *filename) {
    int count = count_processes(filename) + 1;
    pcb_t *processes = insert_pcb(filename);
    return sort_pcb(processes, count);
}

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