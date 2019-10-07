#include <stdio.h>
#include "project3.h"
#include <stdlib.h>

extern int TraceLevel;
#define ME 0

struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};
struct distance_table *dt0;
struct NeighborCosts *neighbor0;
void printdt0( int MyNodeNumber, struct NeighborCosts *neighbor, struct distance_table *dtptr);
int minCosts[4];
struct RoutePacket *previous;
struct RoutePacket *current; 



/* students to write the following two routines, and maybe some others */

void rtinit0() {
    dt0 = (struct distance_table *) malloc(sizeof(struct distance_table));
    neighbor0 = (struct NeighborCosts *) malloc(sizeof(struct NeighborCosts));
    previous = (struct RoutePacket*) malloc(sizeof(struct RoutePacket));
    current = (struct RoutePacket*) malloc(sizeof(struct RoutePacket));
    //initilaize distance table to all infinity (unknown)
    int j;
    int k;
    for(j = 0; j < MAX_NODES; j++){
        for(k = 0; k < MAX_NODES; k++){
            dt0->costs[j][k] = INFINITY;
        }
    }

    // //get costs of neighbors so they they can be processed
    neighbor0 = getNeighborCosts(ME);
    int p;
    for(p = 0; p < MAX_NODES; p++){
        printf("Node0 info: %d\n", neighbor0->NodeCosts[p]);
    }

    int i;
    //int f;
    //for each node cost in neighbor0, put it in the correct location in node0's
    //distance table: 0, 
    //for 0, want to fill in (0, 0), (1, 1), (2, 2), (3, 3)
    //also fill in the mincosts!
    for(i = 0; i < MAX_NODES; i++){
        dt0->costs[i][i] = neighbor0->NodeCosts[i];
        minCosts[i] = neighbor0->NodeCosts[i];
    }
    printdt0(ME, neighbor0, dt0);
    //send information to neighbor nodes; call layer2 for each neighbor
    //neighbors of node 0: 1 2 3

    //get mincosts and place in each packet
    
    struct RoutePacket *packet = (struct RoutePacket *) malloc(sizeof(struct RoutePacket));
    packet->sourceid = ME;
    
    packet->destid = 1;
    int m;
    printf("mincosts calc'd for node0: ");
    for(m= 0; m < 4; m++){
        printf("%d, ", minCosts[m]);
        packet->mincost[m] = minCosts[m];
    }
    printf("\n");
    toLayer2(*packet);
    packet->destid = 2;
    toLayer2(*packet);
    packet->destid = 3;
    toLayer2(*packet);
    //after initializing table, send to distance tables to neighbors in form of update packet
}


void rtupdate0( struct RoutePacket *rcvdpkt ) {
    //format of mincosts is, based on source, to node 0,1,2,3
    printf("Node 0 received a new packet!\n");

    //print out values of the new mincosts
    //copy received packet to current to be processed
    //udpdate the node distance table to reflect info; by the end of the session, each node should have the 
    //same distance table with minimized costs, but they won't be printed the same which is annoying

    current->sourceid = rcvdpkt->sourceid;
    current->destid = rcvdpkt->destid;
    printf("neighbor %d mincosts: ", rcvdpkt->sourceid);
    int i;
    for(i = 0; i < 4; i++){
        printf("%d, ", rcvdpkt->mincost[i]);
        current->mincost[i] = rcvdpkt->mincost[i];
        dt0->costs[rcvdpkt->sourceid][i] = rcvdpkt->mincost[i];
        //if any of these are different, then need to re calculate shortest path to each node
        //change the neighborCosts, not the distance table!
        if(current->mincost[i] != previous->mincost[i]){
            printf("New neighbor cost found\n");
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

    printf("Reprinting node 0 distance table post update:\n");
    printdt0(ME, neighbor0, dt0);
    
    
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

