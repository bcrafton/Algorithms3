/*
 * dynamic_knapsack.h
 *
 *  Created on: Sep 1, 2015
 *      Author: Brian
 */

#ifndef DYNAMIC_KNAPSACK_H_
#define DYNAMIC_KNAPSACK_H_

void knapsack_sort(Item* items, int size, int (*compare)(void* o1, void* o2), void (*print)(void* o));

#endif /* DYNAMIC_KNAPSACK_H_ */
