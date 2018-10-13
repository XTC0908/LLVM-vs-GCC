// File: TwoTasks.c 

#include <stdio.h>
#include "includes.h"
#include <string.h>

#define DEBUG 1

/* Definition of Task Stacks */
/* Stack grows from HIGH to LOW memory */
#define   TASK_STACKSIZE       2048
OS_STK    task1_stk[TASK_STACKSIZE];
OS_STK    task2_stk[TASK_STACKSIZE];
OS_STK    stat_stk[TASK_STACKSIZE];
OS_EVENT  *aSemaphore;
OS_EVENT  *bSemaphore;

/* Definition of Task Priorities */
#define TASK1_PRIORITY      6  // highest priority
#define TASK2_PRIORITY      7
#define TASK_STAT_PRIORITY 12  // lowest priority 

void printStackSize(INT8U prio)
{
    INT8U err;
    OS_STK_DATA stk_data;
    
    err = OSTaskStkChk(prio, &stk_data);
    if (err == OS_NO_ERR) 
    {
        if (DEBUG == 1)
           printf("Task Priority %d - Used: %d; Free: %d\n", 
                   prio, stk_data.OSFree, stk_data.OSUsed);
    }
    else
    {
        if (DEBUG == 1)
           printf("Stack Check Error!\n");    
    }
}

/* Prints a message and sleeps for given time interval */
void task0(void* pdata)
{
  INT8U err;
  INT8U state = 0;
  while (1)
  { 
    OSSemPend(aSemaphore, 0, &err);
    char text0_0[] = "Task 0 - State 0\n";
    char text0_1[] = "Task 0 - State 1\n";
    int i;
    if(state == 0)
    {
      for (i = 0; i < strlen(text0_0); i++)
           putchar(text0_0[i]);
      OSSemPost(bSemaphore);  
      OSSemPost(bSemaphore); 
    }
    else if(state == 1)
    {
      for (i = 0; i < strlen(text0_1); i++)
           putchar(text0_1[i]);
    }   
    state = !state;  
    //OSTimeDlyHMSM(0, 0, 0, 11); // Context Switch to next task

                               // Task will go to the ready state

                               // after the specified delay
  }
}

/* Prints a message and sleeps for given time interval */
void task1(void* pdata)
{
  INT8U err;
  INT8U state = 0;
  while (1)
  { 
    OSSemPend(bSemaphore, 0, &err);
    char text1_0[] = "Task 1 - State 0\n";
    char text1_1[] = "Task 1 - State 1\n";
    int i;
    if(state == 0)
    {
       for (i = 0; i < strlen(text1_0); i++)
            putchar(text1_0[i]);
     }
     else if(state == 1)
     {
       for (i = 0; i < strlen(text1_1); i++)
            putchar(text1_1[i]);
       OSSemPost(aSemaphore);  
       OSSemPost(aSemaphore);  
      } 
     state = !state;     
     OSTimeDlyHMSM(0, 0, 0, 4);
  }
}

/* Printing Statistics */
void statisticTask(void* pdata)
{
    while(1)
    {
        /*printStackSize(TASK1_PRIORITY);
        printStackSize(TASK2_PRIORITY);
        printStackSize(TASK_STAT_PRIORITY);*/
    }
}

/* The main function creates two task and starts multi-tasking */
int main(void)
{
  printf("Lab 3 - Two Tasks\n");
  aSemaphore = OSSemCreate(1);
  bSemaphore = OSSemCreate(0);
  OSTaskCreateExt
    (task0,                        // Pointer to task code
     NULL,                         // Pointer to argument that is
                                   // passed to task
     &task1_stk[TASK_STACKSIZE-1], // Pointer to top of task stack
     TASK1_PRIORITY,               // Desired Task priority
     TASK1_PRIORITY,               // Task ID
     &task1_stk[0],                // Pointer to bottom of task stack
     TASK_STACKSIZE,               // Stacksize
     NULL,                         // Pointer to user supplied memory
                                   // (not needed here)
     OS_TASK_OPT_STK_CHK |         // Stack Checking enabled 
     OS_TASK_OPT_STK_CLR           // Stack Cleared                                 
    );
               
  OSTaskCreateExt
    (task1,                        // Pointer to task code
     NULL,                         // Pointer to argument that is
                                   // passed to task
     &task2_stk[TASK_STACKSIZE-1], // Pointer to top of task stack
     TASK2_PRIORITY,               // Desired Task priority
     TASK2_PRIORITY,               // Task ID
     &task2_stk[0],                // Pointer to bottom of task stack
     TASK_STACKSIZE,               // Stacksize
     NULL,                         // Pointer to user supplied memory
                                   // (not needed here)
     OS_TASK_OPT_STK_CHK |         // Stack Checking enabled 
     OS_TASK_OPT_STK_CLR           // Stack Cleared                       
    );  
    
  if (DEBUG == 1)
  {
    OSTaskCreateExt
      (statisticTask,                // Pointer to task code
       NULL,                         // Pointer to argument that is
                                     // passed to task
       &stat_stk[TASK_STACKSIZE-1],  // Pointer to top of task stack
       TASK_STAT_PRIORITY,           // Desired Task priority
       TASK_STAT_PRIORITY,           // Task ID
       &stat_stk[0],                 // Pointer to bottom of task stack
       TASK_STACKSIZE,               // Stacksize
       NULL,                         // Pointer to user supplied memory
                                     // (not needed here)
       OS_TASK_OPT_STK_CHK |         // Stack Checking enabled 
       OS_TASK_OPT_STK_CLR           // Stack Cleared                              
      );
  }  
    
  OSStart();
  return 0;
}
