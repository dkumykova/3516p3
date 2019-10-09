#include <stdio.h>
#include "project3.h"
#include <stdlib.h>

extern int TraceLevel;
#define ME 3

// struct distance_table {
//   int costs[MAX_NODES][MAX_NODES];
// };
struct distance_table *dt3;
struct NeighborCosts   *neighbor3;
int minCosts[4];
struct RoutePacket *previous;
struct RoutePacket *current; 
void printdt3( int MyNodeNumber, struct NeighborCosts *neighbor, 
		struct distance_table *dtptr );

/* students to write the following two routines, and maybe some others */

void rtinit3() {
    dt3 = (struct distance_table *) malloc(sizeof(struct distance_table));
    neighbor3 = (struct NeighborCosts *) malloc(sizeof(struct NeighborCosts));
    previous = (struct RoutePacket*) malloc(sizeof(struct RoutePacket));
    current = (struct RoutePacket*) malloc(sizeof(struct RoutePacket));
    //initilaize distance table to all infinity (unknown)
    int j;
    int k;
    for(j = 0; j < MAX_NODES; j++){
        for(k = 0; k < MAX_NODES; k++){
            dt3->costs[j][k] = INFINITY;
        }
    }

    // //get costs of neighbors so they they can be processed
    neighbor3 = getNeighborCosts(ME);
    int p;
    printf("At time %f, rtinit3 called\nNode3 neighbor info:", getClockTime());
    for(p = 0; p < MAX_NODES; p++){
        printf("%d, ", neighbor3->NodeCosts[p]);
    }
    printf("\n");

    int i;
    //int f;
    //for each node cost in neighbor0, put it in the correct location in node0's
    //distance table: 0, 
    //for 0, want to fill in (0, 0), (1, 1), (2, 2), (3, 3)
    for(i = 0; i < MAX_NODES; i++){
        dt3->costs[ME][i] = neighbor3->NodeCosts[i];
        //minCosts[i] = neighbor3->NodeCosts[i];
    }
    printdt3(ME, neighbor3, dt3);

    //send information to neighbor nodes; call layer2 for each neighbor
    //neighbors of node 3: 0 2

    //get mincosts and place in each packet
    
    struct RoutePacket *packet = (struct RoutePacket *) malloc(sizeof(struct RoutePacket));
    packet->sourceid = ME;
    packet->destid = 0;
    int m;
    //printf("mincosts calc'd for node3: ");
    for(m= 0; m < 4; m++){
      //  printf("%d, ", minCosts[m]);
        packet->mincost[m] = neighbor3->NodeCosts[m];
    }

    for(k = 0; k < MAX_NODES; k++){
        if(neighbor3->NodeCosts[k] == INFINITY){
            //do nothing
        } else {
            packet->destid = k;
            printf("At time %f, node 0 sends packet to node %d with: %d, %d, %d, %d\n", getClockTime(), k, 
            neighbor3->NodeCosts[0], neighbor3->NodeCosts[1], neighbor3->NodeCosts[2], neighbor3->NodeCosts[3]);
            toLayer2(*packet);
        }
    }
    // printf("At time %f, node 3 sends packet to node 0 with: %d, %d, %d, %d\n", getClockTime(), 
    // neighbor3->NodeCosts[0], neighbor3->NodeCosts[1], neighbor3->NodeCosts[2], neighbor3->NodeCosts[3]);
    // toLayer2(*packet);
    // packet->destid = 2;
    // printf("At time %f, node 3 sends packet to node 2 with: %d, %d, %d, %d\n", getClockTime(), 
    // neighbor3->NodeCosts[0], neighbor3->NodeCosts[1], neighbor3->NodeCosts[2], neighbor3->NodeCosts[3]);
    // toLayer2(*packet);

   
}


void rtupdate3( struct RoutePacket *rcvdpkt ) {
    //if(TraceLevel == 1){
        printf("Node 3 received a new packet from %d with values: ", rcvdpkt->sourceid);
        //printf("neighbor %d mincosts: ", rcvdpkt->sourceid);
    //}
    
    int i, j, k, p;

    int previousRow[4];
    //printf("Value of distance vector for %d before update: ", rcvdpkt->sourceid);
    for(j = 0; j < MAX_NODES; j++){
        previousRow[j] = dt3->costs[rcvdpkt->sourceid][j];
        printf("%d, ", rcvdpkt->mincost[j]);
    }
    printf("\n");

    for(i = 0; i < 4; i++){
        //printf("%d, ", rcvdpkt->mincost[i]);
        dt3->costs[rcvdpkt->sourceid][i] = rcvdpkt->mincost[i];
        
    }
    //printf("\n");


    // printf("Reprinting node 0 distance table post update:\n");
    // printdt0(ME, neighbor0, dt0);
    if(TraceLevel == 1){
        printf("Full distance table for node 3!: \n");
        int m, n;
        for(m = 0; m < MAX_NODES; m++){
            for(n = 0; n < MAX_NODES; n++){
                printf("%d ",  dt3->costs[m][n]);
            }
            printf("\n");
            
        }
    }

        for(k = 0; k < MAX_NODES; k++){
            for(p = 0; p < MAX_NODES; p++){
                //printf("Source %d, Dest %d\n", k, p);
                dt3->costs[k][p] = findShortestPath(dt3, k, p);
                dt3->costs[p][k] = findShortestPath(dt3, k, p);
            }
        }
    

    int x;
    for(x = 0; x < MAX_NODES; x++){
        neighbor3->NodeCosts[x] = dt3->costs[ME][x];
        //printf("%d ",  neighbor3->NodeCosts[x]);
    }
    //printf("\n");
    
    printdt3(ME, neighbor3, dt3);
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

