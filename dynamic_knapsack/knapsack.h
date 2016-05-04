/*
 * knapsack.h
 *
 *  Created on: Aug 31, 2015
 *      Author: Brian
 */

#ifndef KNAPSACK_H_
#define KNAPSACK_H_

#include <stdio.h>
#include <stdlib.h>
#include "c_lib//vector.h"

typedef struct Item{
	int id;
	int value;
	int cost;
} Item;

typedef struct Knapsack{
	int total_value;
	int total_cost;
	// going to make the client say how many knapsack objects at the start ...
	// no vector
	Item* items;
	int* selected; // do not want to place this inside the Item structure
	// just feel as though it is bad design.
	int item_count;
	int max_cost;
} Knapsack;

Knapsack* knapsack_constructor(char* filename);
int value(int index, Knapsack* knapsack);
int cost(int index, Knapsack* knapsack);
int total_value(Knapsack* knapsack);
int total_cost(Knapsack* knapsack);
void select(int index, Knapsack* knapsack);
void unselect(int index, Knapsack* knapsack);
int is_selected(int index, Knapsack* knapsack);
void print_knapsack(Knapsack* knapsack);

#endif /* KNAPSACK_H_ */
