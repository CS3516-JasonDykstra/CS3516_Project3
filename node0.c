#include <stdio.h>
#include <string.h>
#include "project3.h"

#define NODE0 0

extern int TraceLevel;
double clocktime;

struct distance_table {
    int costs[MAX_NODES][MAX_NODES];
};
struct distance_table dt0;
struct NeighborCosts *neighbor0;

/* students to write the following two routines, and maybe some others */
int isNeighbor0(int);
int sendToNeighbors0(int,int[]);

int node0MinCost[MAX_NODES];
int node0LinkCost[MAX_NODES];
int node0SendDV;

void rtinit0() {

    node0LinkCost[0] = 0;
    node0LinkCost[1] = 1;
    node0LinkCost[2] = 3;
    node0LinkCost[3] = 7;

    memcpy(dt0.costs[NODE0],node0LinkCost, sizeof(int[MAX_NODES]));
    memcpy(node0MinCost,node0LinkCost, sizeof(int[MAX_NODES]));
    node0SendDV = NO;

    sendToNeighbors0(NODE0,node0MinCost);

}


void rtupdate0( struct RoutePacket *rcvdpkt ) {
    if(isNeighbor0(rcvdpkt->sourceid)){
        int i;
        for(i = 0; i < MAX_NODES; i++){
            if(node0MinCost[i] > rcvdpkt->mincost[i]){
                int possibleRoute = node0MinCost[rcvdpkt->sourceid]+rcvdpkt->mincost[i];
                if(possibleRoute < node0MinCost[i]){
                    node0MinCost[i] = possibleRoute;
                    node0SendDV = YES;
                }
            }
        }
        memcpy(dt0.costs[NODE0],node0MinCost,sizeof(int[MAX_NODES]));
        memcpy(dt0.costs[rcvdpkt->sourceid],rcvdpkt->mincost,sizeof(int[MAX_NODES]));
    }
    if(node0SendDV) {
        sendToNeighbors0(NODE0, node0MinCost);
        node0SendDV = NO;
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
void printdt0( int MyNodeNumber, struct NeighborCosts *neighbor,
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
}    // End of printdt0

int isNeighbor0(int sourceid){
    return node0LinkCost[sourceid] != INFINITY;
}

void sendrtpkt0(int srcId, int destId, int mincosts[]){
    struct RoutePacket packet;
    int i;
    packet.sourceid = srcId;
    packet.destid = destId;
    // Zero packet mincost
    memset(packet.mincost, 0, sizeof(packet.mincost));

    // Initialize mincost array
    for (i = 0; i < MAX_NODES; i++) {
        packet.mincost[i] = mincosts[i];
    }

    // Send packet to layer 2 and print the confirmation
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

int sendToNeighbors0(int sourceid, int mincost[]){
    int i;
    for(i = 0; i < MAX_NODES; i++){
        if(isNeighbor0(i) && i != sourceid) {
            sendrtpkt0(sourceid, i, mincost);
        }
    }
    return 0;
}