#include <stdio.h>
#include "project3.h"
#include <stdlib.h>

extern int TraceLevel;
#define ME 1
// struct distance_table {
//   int costs[MAX_NODES][MAX_NODES];
// };
struct distance_table *dt1;
struct NeighborCosts   *neighbor1;
int minCosts[4];
struct RoutePacket *previous;
struct RoutePacket *current; 
void printdt1( int MyNodeNumber, struct NeighborCosts *neighbor, 
		struct distance_table *dtptr );

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
        dt1->costs[ME][i] = neighbor1->NodeCosts[i];
        //minCosts[i] = neighbor1->NodeCosts[i];
    }
    printdt1(ME, neighbor1, dt1);

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
        packet->mincost[m] = neighbor1->NodeCosts[m];
    }
    printf("At time %f, node 1 sends packet to node 0 with: %d, %d, %d, %d\n", getClockTime(), 
    neighbor1->NodeCosts[0], neighbor1->NodeCosts[1], neighbor1->NodeCosts[2], neighbor1->NodeCosts[3]);
    toLayer2(*packet);
    packet->destid = 2;
    printf("At time %f, node 1 sends packet to node 2 with: %d, %d, %d, %d\n", getClockTime(), 
    neighbor1->NodeCosts[0], neighbor1->NodeCosts[1], neighbor1->NodeCosts[2], neighbor1->NodeCosts[3]);
    toLayer2(*packet);
    packet->destid = 3;
    printf("At time %f, node 1 sends packet to node 3 with: %d, %d, %d, %d\n", getClockTime(), 
    neighbor1->NodeCosts[0], neighbor1->NodeCosts[1], neighbor1->NodeCosts[2], neighbor1->NodeCosts[3]);
    toLayer2(*packet);
}


void rtupdate1( struct RoutePacket *rcvdpkt ) {
    if(TraceLevel == 1){
        printf("Node 1 received a new packet!\n");
        printf("neighbor %d mincosts: ", rcvdpkt->sourceid);
    }
    int i, j, k, p;

    int previousRow[4];
    printf("Value of distance vector for %d before update: ", rcvdpkt->sourceid);
    for(j = 0; j < MAX_NODES; j++){
        previousRow[j] = dt1->costs[rcvdpkt->sourceid][j];
        printf("%d, ", dt1->costs[rcvdpkt->sourceid][j]);
    }
    printf("\n");

    for(i = 0; i < 4; i++){
        printf("%d, ", rcvdpkt->mincost[i]);
        
        dt1->costs[rcvdpkt->sourceid][i] = rcvdpkt->mincost[i];
        if(previousRow[i] != rcvdpkt->mincost[i]){
            //new values! recalculate all paths
            for(k = 0; k < MAX_NODES; k++){
            for(p = 0; p < MAX_NODES; p++){
                //printf("Source %d, Dest %d\n", k, p);
                dt1->costs[k][p] = findShortestPath(dt1, k, p);
            }
        }
        }
       

    }
    printf("\n");

    if(TraceLevel == 1){
        printf("Full distance table for node 1!: \n");
        int m, n;
        for(m = 0; m < MAX_NODES; m++){
            for(n = 0; n < MAX_NODES; n++){
                printf("%d ",  dt1->costs[m][n]);
            }
            printf("\n");
            
        }
    }

        for(k = 0; k < MAX_NODES; k++){
            for(p = 0; p < MAX_NODES; p++){
                //printf("Source %d, Dest %d\n", k, p);
                dt1->costs[k][p] = findShortestPath(dt1, k, p);
            }
        }
    

     printf("Full distance table for node 1! after recalcs**!: \n");
     int w, x;
     for(w = 0; w < MAX_NODES; w++){
         for(x = 0; x < MAX_NODES; x++){
             printf("%d ",  dt1->costs[w][x]);
         }
         printf("\n");
        
     }
    //int x;
    for(x = 0; x < MAX_NODES; x++){
        neighbor1->NodeCosts[x] = dt1->costs[ME][x];
        printf("%d ",  neighbor1->NodeCosts[x]);
    }
    printf("\n");
    printdt1(ME, neighbor1, dt1);
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

