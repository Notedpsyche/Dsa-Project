#include <stdlib.h>
#include "task_mapper.h"

typedef struct Node{
    int taskId, serverId;
    struct Node* next;
} Node;

static Node **table = NULL;
static int    B = 0;

static int h(int key){ return (key % B + B) % B; }

void mapperInit(int buckets){
    B = buckets;
    table = (Node**)calloc(B, sizeof(Node*));
}

void mapTaskToServer(int taskId, int serverId){
    int i = h(taskId);
    for(Node* p=table[i]; p; p=p->next) if(p->taskId==taskId){ p->serverId=serverId; return; }
    Node* nn = (Node*)malloc(sizeof(Node));
    nn->taskId=taskId; nn->serverId=serverId; nn->next=table[i]; table[i]=nn;
}

int getServerForTask(int taskId){
    int i = h(taskId);
    for(Node* p=table[i]; p; p=p->next) if(p->taskId==taskId) return p->serverId;
    return -1;
}

void migrateTask(int taskId, int toServer){
    int i = h(taskId);
    for(Node* p=table[i]; p; p=p->next) if(p->taskId==taskId){ p->serverId=toServer; return; }
    // if not present, insert (defensive)
    mapTaskToServer(taskId, toServer);
}
