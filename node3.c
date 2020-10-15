#include <stdio.h>
#include <string.h>
#include "project3.h"

extern int TraceLevel;
double clocktime;

#define NODE3 3

struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};
struct distance_table dt3;
struct NeighborCosts   *neighbor3;

/* students to write the following two routines, and maybe some others */
int isNeighbor3(int);
int sendToNeighbors3(int,int[]);

int node3MinCost[MAX_NODES];
int node3LinkCost[MAX_NODES];
int node3SendDV;

void rtinit3() {
    node3LinkCost[0] = 7;
    node3LinkCost[1] = INFINITY;
    node3LinkCost[2] = 2;
    node3LinkCost[3] = 0;

    memcpy(dt3.costs[NODE3],node3LinkCost,sizeof(int[MAX_NODES]));
    memcpy(node3MinCost,node3LinkCost,sizeof(int[MAX_NODES]));
    node3SendDV = NO;

    sendToNeighbors3(NODE3, node3MinCost);

}


void rtupdate3( struct RoutePacket *rcvdpkt ) {
    if(isNeighbor3(rcvdpkt->sourceid)){
        int i;
        for(i = 0; i < MAX_NODES; i++){
            if(node3MinCost[i] > rcvdpkt->mincost[i]){
                int possibleRoute = node3MinCost[rcvdpkt->sourceid]+rcvdpkt->mincost[i];
                if(possibleRoute < node3MinCost[i]){
                    node3MinCost[i] = possibleRoute;
                    node3SendDV = YES;
                }
            }
        }
        memcpy(dt3.costs[NODE3],node3MinCost,sizeof(int[MAX_NODES]));
        memcpy(dt3.costs[rcvdpkt->sourceid],rcvdpkt->mincost,sizeof(int[MAX_NODES]));
    }
    if(node3SendDV){
        sendToNeighbors3(NODE3,node3MinCost);
        node3SendDV = NO;
    }

}


/////////////////////////////////////////////////////////////////////
//  printdt
//  This routine is being supplied to you.  It is the same code in
//  each node and is tailored based on the input arguments.
//  Required arguments:
//  MyNodeNumber:  This routine assumes that you know your node
//                 number and supply it when making this call.
//  struct NeighborCosts *neighbor:  A pointer to the structure 
//                 that's supplied via a call to getNeighborCosts().
//                 It tells this print routine the configuration
//                 of nodes surrounding the node we're working on.
//  struct distance_table *dtptr: This is the running record of the
//                 current costs as seen by this node.  It is 
//                 constantly updated as the node gets new
//                 messages from other nodes.
/////////////////////////////////////////////////////////////////////
void printdt3( int MyNodeNumber, struct NeighborCosts *neighbor, 
		struct distance_table *dtptr ) {
    int       i, j;
    int       TotalNodes = neighbor->NodesInNetwork;     // Total nodes in network
    int       NumberOfNeighbors = 0;                     // How many neighbors
    int       Neighbors[MAX_NODES];                      // Who are the neighbors

    // Determine our neighbors 
    for ( i = 0; i < TotalNodes; i++ )  {
        if (( neighbor->NodeCosts[i] != INFINITY ) && i != MyNodeNumber )  {
            Neighbors[NumberOfNeighbors] = i;
            NumberOfNeighbors++;
        }
    }
    // Print the header
    printf("                via     \n");
    printf("   D%d |", MyNodeNumber );
    for ( i = 0; i < NumberOfNeighbors; i++ )
        printf("     %d", Neighbors[i]);
    printf("\n");
    printf("  ----|-------------------------------\n");

    // For each node, print the cost by travelling thru each of our neighbors
    for ( i = 0; i < TotalNodes; i++ )   {
        if ( i != MyNodeNumber )  {
            printf("dest %d|", i );
            for ( j = 0; j < NumberOfNeighbors; j++ )  {
                    printf( "  %4d", dtptr->costs[i][Neighbors[j]] );
            }
            printf("\n");
        }
    }
    printf("\n");
}    // End of printdt3

int isNeighbor3(int sourceid){
    return node3LinkCost[sourceid] != INFINITY;
}

void sendrtpkt3(int srcId, int destId, int mincosts[]){
    struct RoutePacket packet;
    int i;
    packet.sourceid = srcId;
    packet.destid = destId;
    // Zero packet mincost
    memset(packet.mincost, 0, sizeof(packet.mincost));

    for (i = 0; i < MAX_NODES; i++) {
        packet.mincost[i] = mincosts[i];
    }

    toLayer2(packet);
    printf("At time t=%.3f, node %d sends packet to node %d with: (%d  %d  %d  %d)\n",
           clocktime,
           packet.sourceid,
           packet.destid,
           packet.mincost[0],
           packet.mincost[1],
           packet.mincost[2],
           packet.mincost[3]);
}

int sendToNeighbors3(int sourceid, int mincost[]){
    int i;
    for(i = 0; i < MAX_NODES; i++){
        if(isNeighbor3(i) && i != sourceid){
            sendrtpkt3(sourceid, i, mincost);
        }
    }
    return 0;
}