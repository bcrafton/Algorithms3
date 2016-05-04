param numColors;
param numNodes;
set edgeList dimen 2;

param edge{i in 0..numNodes-1, j in 0..numNodes-1} := (if(i,j) in edgeList or (j,i) in edgeList then 1 else 0);
var assign{i in 0..numNodes-1, j in 0..numColors-1} binary;
var conflict{i in 0..numNodes-1, j in 0..numNodes-1} binary; 

minimize conflict_count :
sum{i in 0..numNodes-1, j in 0..numNodes-1} conflict[i,j]/2;

subject to conflicts{i in 0..numNodes-1, j in 0..numNodes-1, k in 0..numColors-1}:
conflict[i,j] >= edge[i,j] + assign[i,k] + assign[j,k] - 2;

subject to max_colors{i in 0..numNodes-1}:
sum{j in 0..numColors-1} assign[i,j] = 1;

