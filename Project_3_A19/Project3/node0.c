#include <stdio.h>
#include "project3.h"
#include <stdlib.h>
#include <time.h>

extern int TraceLevel;
#define ME 0

// struct distance_table {
//   int costs[MAX_NODES][MAX_NODES];
// };
struct distance_table *dt0;
struct NeighborCosts *neighbor0;
void printdt0( int MyNodeNumber, struct NeighborCosts *neighbor, struct distance_table *dtptr);
int minCosts[4];
struct RoutePacket *previous;
struct RoutePacket *current; 
//void findShortestPath(struct RoutePacket *packet);



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
    //float time = getClockTime();
    printf("At time %f, rtinit0 called\nNode0 neighbor info:", getClockTime());
    for(p = 0; p < MAX_NODES; p++){
        printf("%d, ", neighbor0->NodeCosts[p]);
    }
    printf("\n");

    int i;
    //int f;
    //for each node cost in neighbor0, put it in the correct location in node0's
    //distance table: 0, 
    //for 0, want to fill in (0, 0), (1, 1), (2, 2), (3, 3)
    //also fill in the mincosts!
    for(i = 0; i < MAX_NODES; i++){
        dt0->costs[ME][i] = neighbor0->NodeCosts[i];
        //minCosts[i] = neighbor0->NodeCosts[i];
    }
    printdt0(ME, neighbor0, dt0);
    //send information to neighbor nodes; call layer2 for each neighbor
    //neighbors of node 0: 1 2 3

    //get mincosts and place in each packet
    
    struct RoutePacket *packet = (struct RoutePacket *) malloc(sizeof(struct RoutePacket));
    packet->sourceid = ME;
    
    packet->destid = 1;
    int m;
    //printf("mincosts calc'd for node0: ");
    for(m= 0; m < 4; m++){
        //printf("%d, ", minCosts[m]);
        packet->mincost[m] = neighbor0->NodeCosts[m];
    }
    
    for(k = 0; k < MAX_NODES; k++){
        if(neighbor0->NodeCosts[k] == INFINITY){
            //do nothing
        } else {
            packet->destid = k;
            printf("At time %f, node 0 sends packet to node %d with: %d, %d, %d, %d\n", getClockTime(), k, 
            neighbor0->NodeCosts[0], neighbor0->NodeCosts[1], neighbor0->NodeCosts[2], neighbor0->NodeCosts[3]);
            toLayer2(*packet);
        }
    }
    // printf("At time %f, node 0 sends packet to node 1 with: %d, %d, %d, %d\n", getClockTime(), 
    // neighbor0->NodeCosts[0], neighbor0->NodeCosts[1], neighbor0->NodeCosts[2], neighbor0->NodeCosts[3]);
    // toLayer2(*packet);
    // packet->destid = 2;
    // printf("At time %f, node 0 sends packet to node 2 with: %d, %d, %d, %d\n", getClockTime(), 
    // neighbor0->NodeCosts[0], neighbor0->NodeCosts[1], neighbor0->NodeCosts[2], neighbor0->NodeCosts[3]);
    // toLayer2(*packet);
    // packet->destid = 3;
    // printf("At time %f, node 0 sends packet to node 3 with: %d, %d, %d, %d\n", getClockTime(), 
    // neighbor0->NodeCosts[0], neighbor0->NodeCosts[1], neighbor0->NodeCosts[2], neighbor0->NodeCosts[3]);
    // toLayer2(*packet);
    //after initializing table, send to distance tables to neighbors in form of update packet
}


void rtupdate0( struct RoutePacket *rcvdpkt ) {
    //format of mincosts is, based on source, to node 0,1,2,3
    //if(TraceLevel == 1){
        printf("Node 0 received a new packet from %d with values: ", rcvdpkt->sourceid);
        //printf("neighbor %d mincosts: ", rcvdpkt->sourceid);
    //}

    //print out values of the new mincosts
    //copy received packet to current to be processed
    //udpdate the node distance table to reflect info; by the end of the session, each node should have the 
    //same distance table with minimized costs, but they won't be printed the same which is annoying

    int i, j, k, p;

    int previousRow[4];
    //printf("Value of distance vector for %d before update: ", rcvdpkt->sourceid);
    for(j = 0; j < MAX_NODES; j++){
        previousRow[j] = dt0->costs[rcvdpkt->sourceid][j];
        printf("%d, ", rcvdpkt->mincost[j]);
    }
    printf("\n");

    for(i = 0; i < 4; i++){
     //   printf("%d, ", rcvdpkt->mincost[i]);
        dt0->costs[rcvdpkt->sourceid][i] = rcvdpkt->mincost[i];

    }
    //printf("\n");

    // printf("Reprinting node 0 distance table post update:\n");
    // printdt0(ME, neighbor0, dt0);
    if(TraceLevel == 1){
        printf("Full distance table for node 0! before any recalcs!: \n");
        int m, n;
        for(m = 0; m < MAX_NODES; m++){
            for(n = 0; n < MAX_NODES; n++){
                printf("%d ",  dt0->costs[m][n]);
            }
            printf("\n");
            
        }
    }

        for(k = 0; k < MAX_NODES; k++){
            for(p = 0; p < MAX_NODES; p++){
                //printf("Source %d, Dest %d\n", k, p);
                dt0->costs[k][p] = findShortestPath(dt0, k, p);
                dt0->costs[p][k] = findShortestPath(dt0, k, p);
            }
        }
    
    int x;
    for(x = 0; x < MAX_NODES; x++){
        neighbor0->NodeCosts[x] = dt0->costs[ME][x];
       // printf("%d ",  neighbor0->NodeCosts[x]);
    }

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

// void findShortestPath(struct RoutePacket *packet){
//     //given the source and destination nodes, find shortest path between by getting their neighbor costs. 
//     //update node 0 neighbor costs and distance table with new info

//     //this is based entirely off of the distance table!!!
//     //ex. node 0
//     int minCost = 9999;
//     int directCost = 0;
//     int cost = 0;
//     int i, j, k, m, n;

//     //cost directly from source to dest
//     directCost = neighbor0->NodeCosts[packet->destid];
//     if(directCost < minCost){
//         minCost = directCost;
//         printf("Cost is: %d\n", minCost);
//         printf("Direct Path: %d, %d\n", packet->sourceid, packet->destid);
//     }

//     //source --> intermediate --> destination
//     for(i = 0; i < MAX_NODES; i++){
//         if(i != packet->sourceid && i != packet->destid){
//             cost = neighbor0->NodeCosts[i] + dt0->costs[i][packet->destid];
//             if(cost < minCost){
//                 minCost = cost;
//                 printf("Cost is: %d\n", minCost);
//                 printf("One hop path: %d, %d, %d\n", packet->sourceid, i, 
//                     packet->destid);
//             }
//         }
//     }

//     //source --> intermediate --> intermediate --> destination

//     for(j = 0; j < MAX_NODES; j++){
//         if(j != packet->sourceid && j != packet->destid){
//             for(k = 0; k < MAX_NODES; k++){
//                 if(k != packet->sourceid && k != packet->destid){
//                     cost = neighbor0->NodeCosts[j] + dt0->costs[j][k] + 
//                         dt0->costs[k][packet->destid];
//                     if(cost < minCost){
//                         minCost = cost;
//                         printf("Cost is: %d\n", minCost);
//                         printf("Two hop path: %d, %d, %d, %d\n", 
//                             packet->sourceid, j, k, packet->destid);
//                     }
//                 }
//             }
//         }
//     }
// }

