#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "task_scheduler.h"
#include "server_manager.h"
#include "task_mapper.h"
#include "event_simulator.h"

// Simple demo: create tasks, schedule arrivals, assign to least loaded server, complete later.

int main(){
    int N_SERVERS = 3;
    int N_TASKS   = 8;

    // ---- init modules ----
    initTaskScheduler(N_TASKS + 10);     // Rafan (Max-Heap)
    initServerManager(N_SERVERS);        // Ruel  (Min-Heap)
    mapperInit(257);                     // Saivarad (HashMap)
    initEventQueue(N_TASKS*3 + 10);      // Kevin (Min-Heap of events)
    metricsInit();

    // ---- seed tasks into Rafan's PQ ----
    // (You can replace this with user input or random generation)
    for(int i=1;i<=N_TASKS;i++){
        Task t;
        t.taskId   = i;
        t.priority = (i*7)%10 + 1;     // 1..10
        t.size     = (i*3)%7 + 2;      // 2..8
        t.deadline = 10 + i;           // arbitrary
        insertTask(t);

        // schedule an ARRIVAL event at time = i (staggered)
        Event a = { .time=i, .eventType=ARRIVAL, .taskId=t.taskId, .serverId=-1, .duration=0 };
        scheduleEvent(a);
    }

    // ---- simulation loop ----
    while(!isEventQueueEmpty()){
        Event e = nextEvent();

        if(e.eventType == ARRIVAL){
            if(isTaskQueueEmpty()){ continue; }
            Task t = extractHighestPriority();

            int sid = getLeastLoadedId();
            mapTaskToServer(t.taskId, sid);

            updateServerLoad(sid, t.size);   // server load increases
            metricsOnAssign(e.time);

            // schedule completion
            Event c = { .time = e.time + t.size, .eventType = COMPLETION,
                        .taskId = t.taskId, .serverId = sid, .duration = t.size };
            scheduleEvent(c);

            printf("[t=%d] ARRIVAL: Task %d (prio=%d,size=%d) -> Server %d (new load)\n",
                    e.time, t.taskId, t.priority, t.size, sid);
        }
        else if(e.eventType == COMPLETION){
            updateServerLoad(e.serverId, -e.duration);
            metricsOnComplete(e.time);
            printf("[t=%d] COMPLETE: Task %d finished on Server %d\n",
                    e.time, e.taskId, e.serverId);
        }
        // MIGRATION path is optional for v1 (can be added later)
    }

    // ---- end ----
    printf("\nLeast-loaded server at the end: #%d (load=%d)\n",
            getLeastLoadedId(), peekLeastLoaded().load);
    printFinalStats();
    return 0;
}
