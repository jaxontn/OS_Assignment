#include "pp.h"


int main()
{
    char fileName[10]; /*file name no longer that 10 characters*/

    do
    {
        printf("exclude \".txt\" from file name\nPP simulation: ");
        scanf("%s", fileName);
        if( strcmp(fileName, QUIT) != 0 )
        {
            readFile( fileName ); /*read file when received file name*/
        }
    }while( strcmp(fileName, QUIT) != 0 );

    return 0;
}






void readFile( char fileName[] )
{
    Task task; /*to declare the Task struct for usage*/
    int time, burst_t, priority, task_count, pid;
    FILE *f;

    strcat(fileName, ".txt");    
    f = fopen(fileName, "r");

    if( f == NULL )
    {
        perror("Error opening file");
    }
    else
    {
        task_count = 0;
        pid = 'A';
        while( fscanf(f, "%d %d %d", &time, &burst_t, &priority) !=EOF )
        {
            task[task_count].pid = pid;
            task[task_count].time = time;
            task[task_count].burst_t = burst_t;
            task[task_count].priority = priority;
            task[task_count].rt = burst_t;
            task[task_count].ct = NONE;
            task[task_count].tat = NONE;
            task[task_count].wt = NONE;

            task_count++;
            pid++;
        }
        if( ferror(f) )
        {
            perror("Error reading file");
        }
        sortArrivalTime( task_count, task );
        ppStart( task_count, task ); /*start the pp scheduling*/
        displayDetails( task_count, task ); /*display table & results*/
    }
    fclose(f);
}





/*sorting arrival time from 0 to n (increasing order)*/
void sortArrivalTime( int task_count, Task task )
{
    Task tempTask;
    int i, j;

    i = 0;
    while( i < task_count - 1 )
    {
        j = i + 1;
        while( j < task_count )
        {
            if( task[i].time > task[j].time )
            {/*swap places if time j is smaller than time i*/
                tempTask[0] = task[i];
                task[i] = task[j];
                task[j] = tempTask[0];
            }
            j++;
        }
        i++;
    }
}






void displayDetails( int task_count, Task task )
{
    int i;
    double avgWT, avgTAT, totalWT, totalTAT;
    printf("Number of Task: %d\n\n", task_count);
    printf("pid\ttime\tburst\tpriority   rt\tct\ttat\twt\n");
    printf("---\t----\t-----\t--------   --\t--\t---\t--\n");

    totalWT = 0;
    totalTAT = 0;
    for( i = 0; i < task_count; i++ )/*display tasks data*/
    {
        printf("%c\t", task[i].pid);
        printf("%d\t", task[i].time);
        printf("%d\t", task[i].burst_t);
        printf("%d\t", task[i].priority);
        printf("   %d\t", task[i].rt);
        printf("%d\t", task[i].ct);
        printf("%d\t", task[i].tat);
        printf("%d\n", task[i].wt);

        /*calculate total waiting time and turnaround time*/
        totalWT += task[i].wt;
        totalTAT += task[i].tat;
    }
    /*calculte average waiting time and turnaround time*/
    avgWT = totalWT / task_count; 
    avgTAT = totalTAT / task_count;

    printf("\nAVERAGE WAITING TIME: %f\n", avgWT);
    printf("\nAVERAGE TURNAROUND TIME: %f\n", avgTAT);
}





void ppStart( int task_count, Task task )
{
    Task readyQueue, running;
    int curTime, i, ready_count, burst_count;
    curTime = 0;
    running[0].empty = true;

    /*find total burst time*/
    burst_count = findTotalBurst( task_count, task );
    printf("Total burst time: %d\n", burst_count);

    /*displaying Gantt Chart-------------------------------------------*/
    printf("|");
    ready_count = 0;
    do
    {
        /*1. find the new task on current time*/
        ready_count = findNewTask( task_count, task, readyQueue, curTime,
                                   ready_count );
        /*2. find the highest task*/
        ready_count = findHighest( readyQueue, running, ready_count,
                                   &burst_count );
        /*3. decrement the remaining time and/or removing the running to
             mark it as empty when running is done*/
        ready_count = decrementRT( task_count, task, readyQueue, running,
                                   curTime, ready_count );
        printf("|");
        curTime++; /*go to next time*/
    }while( curTime < burst_count ); /*loop through each time*/
    printf("\n"); 

    for( i = 0; i <= burst_count; i++ ) /*for Gantt Chart time interval*/
        printf("%d ", i );
    printf("\n");
    /*-----------------------------------------------------------------*/
}






int findTotalBurst( int task_count, Task task )
{
    /*find total burst time*/
    int i;
    int burst_count = 0;
    for( i = 0; i < task_count; i++ )
    {
        burst_count += task[i].burst_t;
    }
    return burst_count;
}





int findNewTask( int task_count, Task task, Task readyQueue, int curTime,
                  int ready_count )
{
    int i;
    for( i = 0; i < task_count; i++ )
    {
        if( task[i].time == curTime ) /*find new task for current time*/
        {
            readyQueue[ready_count] = task[i]; /*insert to ready queue*/
            readyQueue[ready_count].empty = false;
            ready_count++;
        }
    }
    return ready_count;
}






int findHighest( Task readyQueue, Task running, int ready_count, 
                 int *burst_count )
{
     int i, highPrioNum, highPrioIdx;

     /*now check which process in the queue is highest*/
     if( ready_count >= ONE )/*have multiple task to choose from*/
     {
         highPrioNum = readyQueue[0].priority; /*initial priority NUMBER*/
         highPrioIdx = 0; /*initial priority INDEX*/
         for( i = 1; i < ready_count; i++ )
         {
             /*if found another new higher priority*/
             if( highPrioNum > readyQueue[i].priority )
             {
                 /*new higher priority found with different pid*/
                 highPrioNum = readyQueue[i].priority;
                 highPrioIdx = i;
             }
         }
         /*if found same priority, no need preemption, because      
          first come first serve basis, so the current running     
          continue running without changing to another process     
          of same priority*/
         if( !running[0].empty )
         {
             /*only assign process when have higher priority*/
             if( running[0].priority > readyQueue[highPrioIdx].priority )
             {
                 ready_count = determineRun( readyQueue, running, 
                                             ready_count, highPrioIdx );
             }
             else
             {
             }
         }
         else if( running[0].empty )
         {/*only assign process if running is empty*/
             ready_count = determineRun( readyQueue, running, 
                                         ready_count, highPrioIdx );
         }
     }
     else if( ready_count == EMPTY && running[0].empty )
     {
          *burst_count = *burst_count + 1;
     }
     /*if ready_count is empty means no ready queue allowed to enter 
       running process, because empty*/
    return ready_count;
}






int determineRun( Task readyQueue, Task running, int ready_count, 
                  int highPrioIdx )
{
    int i;
    /*determine how processes should run*/
     if( !running[0].empty )
     {/*have running process, need to save progress first*/
        /*means need to preemp the current running process to make way
         for the higher priority task*/

         /*1. save progress by adding to ready queue*/
         readyQueue[ready_count] = running[0];
         ready_count++;

         /*2. change to another process*/
         running[0] = readyQueue[highPrioIdx];
         running[0].highPrioIdx = highPrioIdx;
         running[0].empty = false;

         /*3. delete the "assigned ready queue to running" from the 
              queue and shift the array forward like a queue*/
         for( i = running[0].highPrioIdx; i < ready_count - 1; i++ )
         {                                   
             readyQueue[i] = readyQueue[i+1];
         }                                      
         ready_count--;
     }
     else if( running[0].empty )
     {/*no process, just assign process*/

         /*1. assign a process*/
         running[0] = readyQueue[highPrioIdx];
         running[0].highPrioIdx = highPrioIdx;
         running[0].empty = false; /*means no more empty*/

         /*2. delete the "assigned ready queue to running" from the queue
           and shift the array forward like a queue*/
         for( i = running[0].highPrioIdx; i < ready_count - 1; i++ )
         {
             readyQueue[i] = readyQueue[i+1];
         }
         ready_count--;
    }
    return ready_count;
}






int decrementRT( int task_count, Task task, Task readyQueue,
                 Task running, int curTime, int ready_count )
{
    int count;
    if( !running[0].empty )
    {
        /*if still have remaining time*/
        if( running[0].rt > EMPTY )
        {
            running[0].rt--; /*decrement remaining time of running*/
            printf("%c", running[0].pid);

          /*if reaches zero after decrement, save data, & straight away 
            set running to empty for other process to use*/         
            if( running[0].rt == EMPTY )/*if no more remaining time*/
            {
                /*1. calculate completion, turnaround, waiting time*/
                running[0].ct = curTime + 1; /*because get next time*/
                running[0].tat = running[0].ct - running[0].time;
                running[0].wt = running[0].tat - running[0].burst_t;

                /*2. find original task (not running task or ready queue)
                     to update their rt, ct, tat, wt*/
                for( count = 0; count < task_count; count++ )
                {
                    /*found the running procoess's original/initial task*/
                    if( running[0].pid == task[count].pid )
                    {
                        /*3. transfer the info to original task from 
                             running process*/
                        task[count].rt = running[0].rt;
                        task[count].ct = running[0].ct;
                        task[count].tat = running[0].tat;
                        task[count].wt = running[0].wt;
                        running[0].empty = true;/*4. set running to empty*/
                    } 
                }
            }
        }
    }
    return ready_count;
}
