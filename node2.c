#include <stdio.h>
#include <string.h>
#include "project3.h"

extern int TraceLevel;
double clocktime;

#define NODE2 2

struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};
struct distance_table dt2;
struct NeighborCosts   *neighbor2;

/* students to write the following two routines, and maybe some others */
int isNeighbor2(int);
int sendToNeighbors2(int,int[]);

int node2MinCost[MAX_NODES];
int node2LinkCost[MAX_NODES];
int node2SendDV;

void rtinit2() {
    node2LinkCost[0] = 3;
    node2LinkCost[1] = 1;
    node2LinkCost[2] = 0;
    node2LinkCost[3] = 2;

    memcpy(dt2.costs[NODE2],node2LinkCost,sizeof(int[MAX_NODES]));
    memcpy(node2MinCost,node2LinkCost,sizeof(int[MAX_NODES]));
    node2SendDV = NO;

    sendToNeighbors2(NODE2, node2MinCost);

}


void rtupdate2( struct RoutePacket *rcvdpkt ) {
    if(isNeighbor2(rcvdpkt->sourceid)){
        int i;
        for(i = 0; i < MAX_NODES; i++){
            if(node2MinCost[i] > rcvdpkt->mincost[i]){
                int possibleRoute = node2MinCost[rcvdpkt->sourceid] + rcvdpkt->mincost[i];
                if(possibleRoute < node2MinCost[i]){
                    node2MinCost[i] = possibleRoute;
                    node2SendDV = YES;
                }
            }
        }
        memcpy(dt2.costs[NODE2],node2MinCost,sizeof(int[MAX_NODES]));
        memcpy(dt2.costs[rcvdpkt->sourceid],rcvdpkt->mincost,sizeof(int[MAX_NODES]));
    }
    if(node2SendDV){
        sendToNeighbors2(NODE2,node2MinCost);
        node2SendDV = NO;
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
void printdt2( int MyNodeNumber, struct NeighborCosts *neighbor, 
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
}    // End of printdt2

int isNeighbor2(int sourceid){
    return node2LinkCost[sourceid] != INFINITY;
}

void sendrtpkt2(int srcId, int destId, int mincosts[]){
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

int sendToNeighbors2(int sourceid, int mincost[]){
    int i;
    for(i = 0; i < MAX_NODES; i++){
        if(isNeighbor2(i) && i != sourceid){
            sendrtpkt2(sourceid, i, mincost);
        }
    }
    return 0;
}