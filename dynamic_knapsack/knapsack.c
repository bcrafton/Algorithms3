#include "knapsack.h"

Knapsack* knapsack_constructor(char* filename){
	FILE* knapsack_file = fopen(filename, "r");
	int item_count;
	int max_cost;
	fscanf(knapsack_file, "%d", &item_count);
	fscanf(knapsack_file, "%d", &max_cost);
	Knapsack *knapsack = malloc(sizeof(Knapsack));
	knapsack->item_count = item_count;
	knapsack->max_cost = max_cost;
	knapsack->items = malloc(sizeof(Item) * item_count);
	knapsack->selected = malloc(sizeof(int) * item_count);
	knapsack->total_cost = 0;
	knapsack->total_value = 0;

	memset(knapsack->selected, 0, knapsack->item_count * sizeof(int));
	int itemCounter = 0;
	for(itemCounter = 0; itemCounter < item_count; itemCounter++){
		fscanf(knapsack_file, "%d %d %d", &knapsack->items[itemCounter].id, &knapsack->items[itemCounter].value, &knapsack->items[itemCounter].cost);
	}
	return knapsack;
}
int value(int index, Knapsack* knapsack){
	Item* item = vector_get(index, knapsack->items);
	return item->value;
}
int cost(int index, Knapsack* knapsack){
	Item* item = vector_get(index, knapsack->items);
	return item->cost;
}
int total_value(Knapsack* knapsack){
	return knapsack->total_value;
}
int total_cost(Knapsack* knapsack){
	return knapsack->total_cost;
}
// important to use these functions since they maintain the total_cost, total_value
// invariants.
void select(int index, Knapsack* knapsack){
	if(knapsack->selected[index] == 0){
		knapsack->selected[index] = 1;
		knapsack->total_value = knapsack->total_value + value(index, knapsack);
		knapsack->total_cost = knapsack->total_cost + cost(index, knapsack);
	}
}
void unselect(int index, Knapsack* knapsack){
	if(knapsack->selected[index] == 1){
		knapsack->selected[index] = 0;
		knapsack->total_value = knapsack->total_value - value(index, knapsack);
		knapsack->total_cost = knapsack->total_cost - cost(index, knapsack);
	}
}
int is_selected(int index, Knapsack* knapsack){
	return knapsack->selected[index] == 1;
}

void print_knapsack(Knapsack* knapsack){
	int counter;
	for(counter = 0; counter < knapsack->item_count; counter++){
		printf("id: %d selected: %d cost: %d value: %d\n", knapsack->items[counter].id, knapsack->selected[counter], knapsack->items[counter].cost, knapsack->items[counter].value);
	}
}
