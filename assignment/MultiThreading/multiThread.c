#include "multiThread.h"


/*shared memory*/
int countB1, countB2; /*for the buffer*/
char fileName[10]; /*for the writer method*/
Buffer buffer1;
Buffer buffer2;

pthread_mutex_t keyOne = PTHREAD_MUTEX_INITIALIZER;/*mutex for buffer1*/
pthread_mutex_t keyTwo = PTHREAD_MUTEX_INITIALIZER;/*mutex for buffer2*/
pthread_mutex_t uiKey = PTHREAD_MUTEX_INITIALIZER;/*mutex for UI*/

pthread_cond_t isFull = PTHREAD_COND_INITIALIZER;/*if full buffer1*/
pthread_cond_t isEmpty = PTHREAD_COND_INITIALIZER;/*if empty buffer1*/

pthread_cond_t isFullBTwo = PTHREAD_COND_INITIALIZER;/*full buffer2*/
pthread_cond_t isEmptyBTwo = PTHREAD_COND_INITIALIZER;/*empty buffer2*/



int main()
{
    int i; /*for loop*/
    /*two threads*/
    pthread_t ppThread;
    pthread_t srtfThread;
    
    countB1 = 0; /*initial zero file name in buffer*/
    countB2 = 0;
    do
    {
        /*user input with uiKey*/
        pthread_mutex_lock( &uiKey );
        printf("\nInclude \".txt\" after file name\nfile: ");
        scanf("%s", fileName);
        pthread_mutex_unlock( &uiKey );
        /*-----------------------------*/

        /*producer for buffer1****************************************/ 
        pthread_mutex_lock( &keyOne );
        /*means full, need to wait for isEmpty from 2 consumers*/
        while( countB1 == ONE ) /*for condition variable*/
        {
            pthread_cond_wait( &isEmpty, &keyOne );
            /*equivilent to:                  
              pthread_mutex_unlock( &keyOne );
              then wait on signal "isEmpty" then call again
              pthread_mutex_lock( &keyOne );*/
        }
        /*critical section*/
        strcpy(buffer1[BUFFER_SIZE-1].fileName, fileName);
        countB1 = 1;
        pthread_cond_signal( &isFull );/*signal consumer isFull*/
        pthread_mutex_unlock( &keyOne );
        /**************************************************************/

        if( strcmp(fileName, QUIT) != ZERO )
        {
            /*creating child threads: ppThread and srtfThread*/
            /*consumers for buffer1*/
            /*& producers for buffer2*/
            pthread_create(&ppThread, NULL, ppFunc, NULL );
            pthread_create(&srtfThread, NULL, srtfFunc, NULL );
        }
        else if( strcmp(fileName, QUIT) == ZERO )
        {
            /*wait until the thread is done before we exit*/
            if( pthread_join( ppThread, NULL ) == ZERO )
            {
                printf("PP: terminate.\n");
            }
            if( pthread_join( srtfThread, NULL) == ZERO )
            {
                printf("SRTF: terminate.\n");
            }
            pthread_exit(NULL);
            /*forces main function to wait for the threats created to
              complete its work, if not main function will exit too fast*/
        }
        /******get AVG time from buffer2 [consumer]*****************/
        for( i = 0; i < 2; i++ )
        {
            sleep(1);/*to allow time for other sections in the child thread
                       to acquire lock before this section in the parent 
                       thread*/
            pthread_mutex_lock( &uiKey );
            pthread_mutex_lock( &keyTwo );
            /*means empty, need to wait for isFullBTwo from producer*/
            while( countB2 == ZERO )
            {
                pthread_cond_wait( &isFullBTwo, &keyTwo);
            }
            if( buffer2[BUFFER_SIZE-1].type == PP )
            {
                printf("PP: ");
            }
            else if( buffer2[BUFFER_SIZE-1].type == SRTF )
            {
                printf("SRTF: ");
            }
            /*critical seciton*/
            printf("the average turnaround time = %f, ", 
                    buffer2[BUFFER_SIZE-1].avgTAT);
            printf("the average waiting time = %f\n", 
                    buffer2[BUFFER_SIZE-1].avgWT);
            countB2--;
            pthread_cond_signal( &isEmptyBTwo ); /*signal producer 
                                                   isEmptyBTwo*/
            pthread_mutex_unlock( &keyTwo );
            pthread_mutex_unlock( &uiKey );
        }
        /**********************************************************/
    }while( strcmp(fileName, QUIT) != ZERO );
    return 0;
}






void *ppFunc(void *none)
{
    /*consumer for buffer1***************************************/
    pthread_mutex_lock( &keyOne );
    /*means empty, need to wait for isFull from producer*/
    while( countB1 == NONE ) /*for condition variable*/
    {
        pthread_cond_wait( &isFull, &keyOne);
        /*equivilent to:                  
          pthread_mutex_unlock( &keyOne );             
          then wait on signal "isFull" then call again
          pthread_mutex_lock( &keyOne );*/
    }
    /*critical section*/
    srtfReadFile( buffer1[BUFFER_SIZE-1].fileName );
    countB1--;
    pthread_cond_signal( &isEmpty ); /*signal producer isEmpty*/
    pthread_mutex_unlock( &keyOne );
    /***********************************************************/
    return NULL;
}






void *srtfFunc(void *none)
{
    /*consumer for buffer1**************************************/
    pthread_mutex_lock( &keyOne );
    /*means empty, need to wait for isFull from producer*/
    while( countB1 == NONE )
    {
        pthread_cond_wait( &isFull, &keyOne );
        /*equivilent to:                  
          pthread_mutex_unlock( &keyOne );             
          then wait on signal "isFull" then call again
          pthread_mutex_lock( &keyOne );*/
    }
    /*critical section*/
    ppReadFile( buffer1[BUFFER_SIZE-1].fileName );
    countB1--;
    pthread_cond_signal( &isEmpty ); /*signal producer isEmpty*/
    pthread_mutex_unlock( &keyOne );
    /***********************************************************/
    return NULL;
}
