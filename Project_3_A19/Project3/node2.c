#include <stdio.h>
#include "project3.h"
#include <stdlib.h>

extern int TraceLevel;
#define ME 2

// struct distance_table {
//   int costs[MAX_NODES][MAX_NODES];
// };
struct distance_table *dt2;
struct NeighborCosts   *neighbor2;
int minCosts[4];
struct RoutePacket *previous;
struct RoutePacket *current; 
void printdt2( int MyNodeNumber, struct NeighborCosts *neighbor, 
		struct distance_table *dtptr );
/* students to write the following two routines, and maybe some others */

void rtinit2() {
    dt2 = (struct distance_table *) malloc(sizeof(struct distance_table));
    neighbor2 = (struct NeighborCosts *) malloc(sizeof(struct NeighborCosts));
    previous = (struct RoutePacket*) malloc(sizeof(struct RoutePacket));
    current = (struct RoutePacket*) malloc(sizeof(struct RoutePacket));
    //initilaize distance table to all infinity (unknown)
    int j;
    int k;
    for(j = 0; j < MAX_NODES; j++){
        for(k = 0; k < MAX_NODES; k++){
            dt2->costs[j][k] = INFINITY;
        }
    }

    // //get costs of neighbors so they they can be processed
    neighbor2 = getNeighborCosts(ME);
    int p;
    printf("At time %f, rtinit2 called\nNode2 neighbor info:", getClockTime());
    for(p = 0; p < MAX_NODES; p++){
        printf("%d, ", neighbor2->NodeCosts[p]);
    }
    printf("\n");

    int i;
    //int f;
    //for each node cost in neighbor0, put it in the correct location in node0's
    //distance table: 0, 
    //for 0, want to fill in (0, 0), (1, 1), (2, 2), (3, 3)
    for(i = 0; i < MAX_NODES; i++){
        dt2->costs[ME][i] = neighbor2->NodeCosts[i];
        //minCosts[i] = neighbor2->NodeCosts[i];
    }
    printdt2(ME, neighbor2, dt2);

    //send information to neighbor nodes; call layer2 for each neighbor
    //neighbors of node 2: 0 1 3

    //get mincosts and place in each packet
    
    struct RoutePacket *packet = (struct RoutePacket *) malloc(sizeof(struct RoutePacket));
    packet->sourceid = ME;
    packet->destid = 0;
    int m;
    //printf("mincosts calc'd for node2: ");
    for(m= 0; m < 4; m++){
      //  printf("%d, ", minCosts[m]);
        packet->mincost[m] = neighbor2->NodeCosts[m];
    }
    printf("At time %f, node 2 sends packet to node 0 with: %d, %d, %d, %d\n", getClockTime(), 
    neighbor2->NodeCosts[0], neighbor2->NodeCosts[1], neighbor2->NodeCosts[2], neighbor2->NodeCosts[3]);
    toLayer2(*packet);
    packet->destid = 1;
    printf("At time %f, node 2 sends packet to node 1 with: %d, %d, %d, %d\n", getClockTime(), 
    neighbor2->NodeCosts[0], neighbor2->NodeCosts[1], neighbor2->NodeCosts[2], neighbor2->NodeCosts[3]);
    toLayer2(*packet);
    packet->destid = 3;
    printf("At time %f, node 2 sends packet to node 3 with: %d, %d, %d, %d\n", getClockTime(), 
    neighbor2->NodeCosts[0], neighbor2->NodeCosts[1], neighbor2->NodeCosts[2], neighbor2->NodeCosts[3]);
    toLayer2(*packet);
}


void rtupdate2( struct RoutePacket *rcvdpkt ) {
    //if(TraceLevel == 1){
        printf("Node 2 received a new packet from %d with values: ", rcvdpkt->sourceid);
        //printf("neighbor %d mincosts: ", rcvdpkt->sourceid);
    //}
    int i, j, k, p;

    int previousRow[4];
    //printf("Value of distance vector for %d before update: ", rcvdpkt->sourceid);
    for(j = 0; j < MAX_NODES; j++){
        previousRow[j] = dt2->costs[rcvdpkt->sourceid][j];
       printf("%d, ", rcvdpkt->mincost[j]);
    }
    printf("\n");

    for(i = 0; i < 4; i++){
        //printf("%d, ", rcvdpkt->mincost[i]);
        dt2->costs[rcvdpkt->sourceid][i] = rcvdpkt->mincost[i];

    }
   // printf("\n");

    

    // printf("Reprinting node 0 distance table post update:\n");
    // printdt0(ME, neighbor0, dt0);
    if(TraceLevel == 1){
        printf("Full distance table for node 2!: \n");
        int m, n;
        for(m = 0; m < MAX_NODES; m++){
            for(n = 0; n < MAX_NODES; n++){
                printf("%d ",  dt2->costs[m][n]);
            }
            printf("\n");
            
        }
    }

        for(k = 0; k < MAX_NODES; k++){
            for(p = 0; p < MAX_NODES; p++){
                //printf("Source %d, Dest %d\n", k, p);
                dt2->costs[k][p] = findShortestPath(dt2, k, p);
                dt2->costs[p][k] = findShortestPath(dt2, k, p);
            }
        }
    

    int x;
    for(x = 0; x < MAX_NODES; x++){
        neighbor2->NodeCosts[x] = dt2->costs[ME][x];
        //printf("%d ",  neighbor2->NodeCosts[x]);
    }
    //printf("\n");
   
    printdt2(ME, neighbor2, dt2);

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

