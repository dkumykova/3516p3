#include <stdio.h>
#include "project3.h"
#include <stdlib.h>

extern int TraceLevel;
#define ME 1
struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};
struct distance_table *dt1;
struct NeighborCosts   *neighbor1;
int minCosts[4];
struct RoutePacket *previous;
struct RoutePacket *current; 

/* students to write the following two routines, and maybe some others */

void rtinit1() {
    dt1 = (struct distance_table *) malloc(sizeof(struct distance_table));
    neighbor1 = (struct NeighborCosts *) malloc(sizeof(struct NeighborCosts));
    previous = (struct RoutePacket*) malloc(sizeof(struct RoutePacket));
    current = (struct RoutePacket*) malloc(sizeof(struct RoutePacket));
    //initilaize distance table to all infinity (unknown)
    int j;
    int k;
    for(j = 0; j < MAX_NODES; j++){
        for(k = 0; k < MAX_NODES; k++){
            dt1->costs[j][k] = INFINITY;
        }
    }

    // //get costs of neighbors so they they can be processed
    neighbor1 = getNeighborCosts(ME);
    int p;
    printf("At time %f, rtinit1 called\nNode1 neighbor info:", getClockTime());
    for(p = 0; p < MAX_NODES; p++){
        printf("%d, ", neighbor1->NodeCosts[p]);
    }
    printf("\n");

    int i;
    //int f;
    //for each node cost in neighbor0, put it in the correct location in node0's
    //distance table: 0, 
    //for 0, want to fill in (0, 0), (1, 1), (2, 2), (3, 3)
    for(i = 0; i < MAX_NODES; i++){
        dt1->costs[i][i] = neighbor1->NodeCosts[i];
        minCosts[i] = neighbor1->NodeCosts[i];
    }
    printdt0(ME, neighbor1, dt1);

    //send information to neighbor nodes; call layer2 for each neighbor
    //neighbors of node 1: 0 2

    //get mincosts and place in each packet
    
    struct RoutePacket *packet = (struct RoutePacket *) malloc(sizeof(struct RoutePacket));
    packet->sourceid = ME;
    packet->destid = 0;
    int m;
    //printf("mincosts calc'd for node1: ");
    for(m= 0; m < 4; m++){
      //  printf("%d, ", minCosts[m]);
        packet->mincost[m] = minCosts[m];
    }
    printf("At time %f, node 1 sends packet to node 0 with: %d, %d, %d, %d\n", getClockTime(), 
    neighbor1->NodeCosts[0], neighbor1->NodeCosts[1], neighbor1->NodeCosts[2], neighbor1->NodeCosts[3]);
    toLayer2(*packet);
    packet->destid = 2;
    printf("At time %f, node 1 sends packet to node 2 with: %d, %d, %d, %d\n", getClockTime(), 
    neighbor1->NodeCosts[0], neighbor1->NodeCosts[1], neighbor1->NodeCosts[2], neighbor1->NodeCosts[3]);
    toLayer2(*packet);
}


void rtupdate1( struct RoutePacket *rcvdpkt ) {
    printf("Node 1 received a new packet!\n");

    current->sourceid = rcvdpkt->sourceid;
    current->destid = rcvdpkt->destid;
    printf("neighbor %d mincosts: ", rcvdpkt->sourceid);
    int i;
    for(i = 0; i < 4; i++){
        printf("%d, ", rcvdpkt->mincost[i]);
        current->mincost[i] = rcvdpkt->mincost[i];
        dt1->costs[rcvdpkt->sourceid][i] = rcvdpkt->mincost[i];
        //if any of these are different, then need to re calculate shortest path to each node
        if(rcvdpkt->mincost[i] != previous->mincost[i]){
            //Dx(y) = min { C(x,v) + Dv(y)} for each node y ∈ N

        }
    }
    printf("\n");

    //check if the distance is different to what is expected; if so, recalculate mincosts

    //at the end of all processing, make current the previous one

    previous->sourceid = current->sourceid;
    previous->destid = current->destid;
    int k;
    for (k = 0; k < 4; k++)
    {
        previous->mincost[k] = current->mincost[k];
    }

    printf("Reprinting node 1 distance table post update:\n");
    printdt0(ME, neighbor1, dt1);
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

