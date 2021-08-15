#ifndef MULTITHREAD_H
#define MULTITHREAD_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include "pp.h"
#include "srtf.h"

/*shared memory*/
#define BUFFER_SIZE 1
#define NONE -1
#define EMPTY 0
#define QUIT "QUIT"
#define ONE 1
#define ZERO 0
#define PP 3425
#define SRTF 1059

typedef struct Buffer{
    char fileName[10]; /*maximum 10 characters including ".txt"*/
    int type; /*either PP or SRTF scheduling*/
    double avgWT, avgTAT; /*for buffer2*/
}Buffer[BUFFER_SIZE];


void *ppFunc(void *none);
void *srtfFunc(void *none);

#endif
