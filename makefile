CC = gcc

# task-1-30067863 : task-1-30067863.o utils.o queue.o
# 	gcc -g task-1-30067863.o utils.o queue.o -o task-1-30067863 -pthread 

# task-1-30067863.o : task-1-30067863.c utils.h queue.h
# 	gcc -g -Wall -c task-1-30067863.c -pthread

task-2-30067863 : task-2-30067863.o utils.o queue.o
	gcc -g task-2-30067863.o utils.o queue.o -o task-2-30067863 -pthread 

task-2-30067863.o : task-2-30067863.c utils.h queue.h
	gcc -g -Wall -c task-2-30067863.c -pthread

utils.o : utils.c utils.h
	gcc -g -Wall -c utils.c

queue.o : queue.c queue.h
	gcc -g -Wall -c queue.c
