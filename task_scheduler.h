#ifndef TASK_SCHEDULER_H
#define TASK_SCHEDULER_H
#include "structures.h"

// DSA USED: Priority Queue (Max-Heap) of Task by (priority, then earlier deadline)

void  initTaskScheduler(int capacity);
void  insertTask(Task t);          // O(log n)
Task  extractHighestPriority();    // O(log n)
int   isTaskQueueEmpty();          // O(1)

#endif
