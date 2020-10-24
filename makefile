CC = gcc

# task1 : task1.o utils.o queue.o
# 	gcc -g task1.o utils.o queue.o -o task1 -pthread 

# task1.o : task1.c utils.h queue.h
# 	gcc -g -Wall -c task1.c -pthread

task2 : task2.o utils.o queue.o
	gcc -g task2.o utils.o queue.o -o task2 -pthread 

task2.o : task2.c utils.h queue.h
	gcc -g -Wall -c task2.c -pthread

utils.o : utils.c utils.h
	gcc -g -Wall -c utils.c

queue.o : queue.c queue.h
	gcc -g -Wall -c queue.c
