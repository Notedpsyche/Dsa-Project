#ifndef STRUCTURES_H
#define STRUCTURES_H

// ---- Common structs & constants ----
#define MAX_TASKS   1000
#define MAX_SERVERS 32

// Event types
#define ARRIVAL     1
#define COMPLETION  2
#define MIGRATION   3

typedef struct {
    int taskId;
    int priority;   // larger = more urgent
    int size;       // execution time units
    int deadline;   // smaller = earlier deadline
} Task;

typedef struct {
    int serverId;
    int load;       // sum of sizes of running tasks
} Server;

typedef struct {
    int time;       // event time
    int eventType;  // ARRIVAL / COMPLETION / MIGRATION
    int taskId;     // task involved
    int serverId;   // server involved
    int duration;   // used for COMPLETION (-load)
} Event;

#endif
