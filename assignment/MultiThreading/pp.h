#ifndef PP_H
#define PP_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "multiThread.h"

typedef struct PPTask{
    int time, burst_t, priority;
    char pid;
    int rt, ct, tat, wt;
    bool empty; /*only for running process*/
    int highPrioIdx; /*only for decrementRT to use for deleting ready
                       queue array index*/
}PPTask[50]; /*maximum number of task is 50*/

void ppReadFile( char fileName[] );
void ppSortArrivalTime( int task_count, PPTask task );
void ppGetDetails( int task_count, PPTask task );
void ppStart( int task_count, PPTask task );
int ppFindTotalBurst( int task_count, PPTask task );
int ppFindNewTask( int task_count, PPTask task, PPTask readyQueue, int curTime,
                   int ready_count );
int ppFindHighest( PPTask readyQueue, PPTask running, int ready_count,
                   int *burst_count );
int ppDetermineRun( PPTask readyQueue, PPTask running, int ready_count,
                    int highPrioIdx );
int ppDecrementRT( int task_count, PPTask task, PPTask readyQueue,
                   PPTask running, int curTime, int ready_count );
#endif
