#include <stdlib.h>
#include "task_mapper.h"

// Node structure for the linked list in each bucket
typedef struct Node {
    int taskId;         // ID of the task
    int serverId;       // ID of the server assigned to this task
    struct Node* next;  // Pointer to the next node in the bucket
} Node;

// Hash table: array of linked list heads
static Node **table = NULL;  
static int B = 0;            // Number of buckets in the hash table

// Hash function: maps taskId to a bucket index (0 to B-1)
static int h(int key) {
    return (key % B + B) % B;  // Ensures non-negative index
}

// Initialize the hash map with a given number of buckets
void mapperInit(int buckets) {
    B = buckets;
    table = (Node**)calloc(B, sizeof(Node*)); // Allocate array and set all buckets to NULL
}

// Map a task to a server (insert new or update existing mapping)
void mapTaskToServer(int taskId, int serverId) {
    int i = h(taskId);  // Compute the bucket index
    Node* p = table[i];

    // Check if the task already exists in this bucket
    while (p) {
        if (p->taskId == taskId) {
            p->serverId = serverId;  // Update server if task exists
            return;
        }
        p = p->next;
    }

    // Task not found → create new node and insert at head of the list
    Node* nn = (Node*)malloc(sizeof(Node));
    nn->taskId = taskId;
    nn->serverId = serverId;
    nn->next = table[i];   // Insert at the head
    table[i] = nn;
}

// Retrieve the server assigned to a task; returns -1 if not found
int getServerForTask(int taskId) {
    int i = h(taskId);      // Compute bucket index
    Node* p = table[i];

    // Traverse the linked list in the bucket
    while (p) {
        if (p->taskId == taskId) return p->serverId;
        p = p->next;
    }

    return -1;  // Task not found
}

// Move a task to a new server (update mapping)
void migrateTask(int taskId, int toServer) {
    int i = h(taskId);
    Node* p = table[i];

    // Check if task exists in the bucket
    while (p) {
        if (p->taskId == taskId) {
            p->serverId = toServer;  // Update serverId
            return;
        }
        p = p->next;
    }

    // Task not present → insert as new (defensive)
    mapTaskToServer(taskId, toServer);
}
