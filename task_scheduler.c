#include <stdio.h>
#include <stdlib.h>
#include "task_scheduler.h"

// ------ DSA: MAX-HEAP of Task ------
static Task *heap;     // 1-based indexing
static int   sizeH = 0, capH = 0;

static int higher(Task a, Task b) {
    if (a.priority != b.priority) return a.priority > b.priority;
    return a.deadline < b.deadline; // earlier deadline wins tie
}

static void swap(Task *a, Task *b){ Task t=*a; *a=*b; *b=t; }

void initTaskScheduler(int capacity){
    capH = capacity;
    sizeH = 0;
    heap = (Task*)malloc(sizeof(Task)*(capH+1));
}

void insertTask(Task t){
    if(sizeH==capH){ printf("Task PQ full\n"); return; }
    heap[++sizeH] = t;
    for(int i=sizeH; i>1 && higher(heap[i], heap[i/2]); i/=2) swap(&heap[i], &heap[i/2]);
}

Task extractHighestPriority(){
    if(sizeH==0){ Task e={-1,-1,-1,-1}; return e; }
    Task top = heap[1];
    heap[1] = heap[sizeH--];
    for(int i=1;;){
        int l=2*i, r=l+1, best=i;
        if(l<=sizeH && higher(heap[l], heap[best])) best=l;
        if(r<=sizeH && higher(heap[r], heap[best])) best=r;
        if(best==i) break;
        swap(&heap[i], &heap[best]); i=best;
    }
    return top;
}

int isTaskQueueEmpty(){ return sizeH==0; }
