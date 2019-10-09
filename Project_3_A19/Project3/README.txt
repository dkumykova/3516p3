To run the program:

cd into the folder containing the .c source files, .h file and the makefile
run "make"
run ./project3 [tracing level] 
	into command line, or fill in prompt as it pops up


Notes:

Explanation of the distance vector routing algorithm as it is implemented in this program:

Program initializes with a node's neighbor's costs, which are the current node's understanding of the minimum
cost to each of its neighbors. Also initialized is the distance table struct, which contains a 4x4 table of 
minimum costs for each node; only the row corresponding to itself will be initially filled in with anything besides
infinity.

Once all distance table and neighbor costs are intitialized, nodes will start sending packets to each other 
containing updated neighbor information. From this information, the recieving node will recalculate the minimum
cost to each of its neighbor nodes; if the new value is less than the previous, it stores that new minimum cost
and updates its neighborCosts struct as well as its distance table.

    