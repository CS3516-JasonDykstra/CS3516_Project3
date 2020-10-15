#include <stdio.h>
#include <string.h>
#include "project3.h"

extern int TraceLevel;
double clocktime;

#define NODE1 1

struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};
struct distance_table dt1;
struct NeighborCosts   *neighbor1;

/* students to write the following two routines, and maybe some others */
int isNeighbor1(int);
int sendToNeighbors1(int,int[]);

int node1MinCost[MAX_NODES];
int node1linkCost[MAX_NODES];
int node1SendDV;

void rtinit1() {
    node1linkCost[0] = 1;
    node1linkCost[1] = 0;
    node1linkCost[2] = 1;
    node1linkCost[3] = INFINITY;

    memcpy(dt1.costs[NODE1],node1linkCost,sizeof(int[MAX_NODES]));
    memcpy(node1MinCost,node1linkCost,sizeof(int[MAX_NODES]));
    node1SendDV = NO;

    sendToNeighbors1(NODE1, node1MinCost);
}


void rtupdate1( struct RoutePacket *rcvdpkt ) {
    if(isNeighbor1(rcvdpkt->sourceid)){
        int i;
        for(i = 0; i < MAX_NODES; i++){
            if(node1MinCost[i] > rcvdpkt->mincost[i]){
                int possibleRoute = node1MinCost[rcvdpkt->sourceid]+rcvdpkt->mincost[i];
                if(possibleRoute < node1MinCost[i]){
                    node1MinCost[i] = possibleRoute;
                    node1SendDV = YES;
                }
            }
        }
        memcpy(dt1.costs[NODE1],node1MinCost,sizeof(int[MAX_NODES]));
        memcpy(dt1.costs[rcvdpkt->sourceid],rcvdpkt->mincost,sizeof(int[MAX_NODES]));
    }
    if(node1SendDV){
        sendToNeighbors1(NODE1,node1MinCost);
        node1SendDV = NO;
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
void printdt1( int MyNodeNumber, struct NeighborCosts *neighbor, 
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
}    // End of printdt1

int isNeighbor1(int sourceid){
    return node1linkCost[sourceid] != INFINITY;
}

void sendrtpkt1(int srcId, int destId, int mincosts[]){
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

int sendToNeighbors1(int sourceid, int mincost[]){
    int i;
    for(i = 0; i < MAX_NODES; i++){
        if(isNeighbor1(i) && i != sourceid){
            sendrtpkt1(sourceid, i, mincost);
        }
    }
    return 0;
}