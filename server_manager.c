#include <stdio.h>
#include <stdlib.h>
#include "server_manager.h"

// ------ DSA: MIN-HEAP of Server by load ------
// heapS is 1-based index for easier parent/child calculation
static Server heapS[MAX_SERVERS + 1];
static int    posById[MAX_SERVERS]; // serverId -> heap index
static int    nS = 0;

// Comparator: first by load, then by serverId
static int less(Server a, Server b) {
    if (a.load != b.load) return a.load < b.load;
    return a.serverId < b.serverId;
}

// Swap two heap nodes and update positions
static void swapS(int i, int j) {
    Server t = heapS[i];
    heapS[i] = heapS[j];
    heapS[j] = t;

    posById[heapS[i].serverId] = i;
    posById[heapS[j].serverId] = j;
}

// Bubble up
static void up(int i) {
    while (i > 1 && less(heapS[i], heapS[i/2])) {
        swapS(i, i/2);
        i /= 2;
    }
}

// Bubble down
static void down(int i) {
    for (;;) {
        int l = 2*i, r = l+1, b = i;
        if (l <= nS && less(heapS[l], heapS[b])) b = l;
        if (r <= nS && less(heapS[r], heapS[b])) b = r;
        if (b == i) break;
        swapS(i, b);
        i = b;
    }
}

// Initialize server manager with 'count' servers
void initServerManager(int count) {
    nS = count;
    for (int i = 1; i <= nS; i++) {
        heapS[i].serverId = i - 1;  // server IDs: 0..count-1
        heapS[i].load = 0;
        posById[heapS[i].serverId] = i;
    }
    for (int i = nS/2; i >= 1; i--) down(i);
}

// Return least-loaded server (without removing)
Server peekLeastLoaded() {
    return heapS[1];
}

// Return least-loaded server ID
int getLeastLoadedId() {
    return heapS[1].serverId;
}

// Update load of a server and reheapify
void updateServerLoad(int serverId, int deltaLoad) {
    int i = posById[serverId];
    heapS[i].load += deltaLoad;

    // Smarter fix: use direction of load change
    if (deltaLoad > 0) down(i);
    else if (deltaLoad < 0) up(i);
    // if deltaLoad==0, do nothing
}

// Return total server count
int serverCount() {
    return nS;
}
