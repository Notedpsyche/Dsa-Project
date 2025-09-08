#ifndef TASK_MAPPER_H
#define TASK_MAPPER_H

// DSA USED: Hash Table (separate chaining) for taskId -> serverId

void mapperInit(int buckets);
void mapTaskToServer(int taskId, int serverId);
int  getServerForTask(int taskId);          // returns -1 if not found
void migrateTask(int taskId, int toServer); // updates mapping

#endif
