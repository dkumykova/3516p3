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

node 0:
get costs from neighbors 1, 2, 3
set neighborCosts
set distance table with own row
node 0 receives mincosts from node 1 and compares to the row it has for node 1 in its distance table; if any
of the values are smaller than what node 0 has, it replaces those values with the new ones
    -once it updates the distance table, it needs to recalculate its own neighborCosts --> algorithm
    Dx(y) = min { C(x,v) + Dv(y)} for each node y ∈ N
    neighborCost(base 0, to 2) = min {C(0,2)} = min{(C(0,2), C(0,1) + C(1,2), C(0, 3) + C(3, 2))}