#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H
#include "structures.h"

// DSA USED: Min-Heap by Server.load (least-loaded server on top)

void   initServerManager(int serverCount);  // creates servers [0..serverCount-1]
Server  peekLeastLoaded();                  // O(1)
int     getLeastLoadedId();                 // O(1)
void    updateServerLoad(int serverId, int deltaLoad); // O(log n)
int     serverCount();

#endif
