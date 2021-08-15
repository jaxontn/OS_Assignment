#ifndef SRTF_H
#define SRTF_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "multiThread.h"

typedef struct SRTFTask{
    int time, burst_t, priority;
    char pid;
    int rt, ct, tat, wt;
    bool empty; /*only for running process*/
    int highPrioIdx; /*only for decrementRT to use for deleting ready
                       queue array index*/
}SRTFTask[50]; /*maximum number of task is 50*/

void srtfReadFile( char fileName[] );
void srtfSortArrivalTime( int task_count, SRTFTask task );
void srtfGetDetails( int task_count, SRTFTask task );
void srtfStart( int task_count, SRTFTask task );
int srtfFindTotalBurst( int task_count, SRTFTask task );
int srtfFindNewTask( int task_count, SRTFTask task, SRTFTask readyQueue, 
                     int curTime, int ready_count );
int srtfFindHighest( SRTFTask readyQueue, SRTFTask running, int ready_count,
                     int *burst_count );
int srtfDetermineRun( SRTFTask readyQueue, SRTFTask running, int ready_count,
                      int highPrioIdx );
int srtfDecrementRT( int task_count, SRTFTask task, SRTFTask readyQueue,
                     SRTFTask running, int curTime, int ready_count );
#endif
