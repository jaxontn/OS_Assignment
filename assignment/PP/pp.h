#ifndef PP_H
#define PP_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define NONE -1
#define EMPTY 0
#define QUIT "QUIT"
#define ONE 1

typedef struct Task{
    int time, burst_t, priority;
    char pid;
    int rt, ct, tat, wt;
    bool empty; /*only for running process*/
    int highPrioIdx; /*only for decrementRT to use for deleting ready
                       queue array index*/
}Task[50]; /*maximum number of task is 50*/


void readFile( char fileName[] );
void sortArrivalTime( int task_count, Task task );
void displayDetails( int task_count, Task task );
void ppStart( int task_count, Task task );
int findTotalBurst( int task_count, Task task );
int findNewTask( int task_count, Task task, Task readyQueue, int curTime,
                  int ready_count );
int findHighest( Task readyQueue, Task running, int ready_count,
                 int *burst_count );
int determineRun( Task readyQueue, Task running, int ready_count,
                  int highPrioIdx );
int decrementRT( int task_count, Task task, Task readyQueue,
                  Task running, int curTime, int ready_count );
#endif
