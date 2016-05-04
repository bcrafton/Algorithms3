param n;
param value{0..n-1};
param cost{0..n-1};
param costBound;
var sack{0..n-1} binary;

maximize total_value: sum{i in 0..n-1} value[i] * sack[i];

subject to max_cost:
sum{j in 0..n-1} cost[j] * sack[j] <= costBound;
