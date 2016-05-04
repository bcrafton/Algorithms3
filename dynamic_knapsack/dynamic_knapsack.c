/*
 ============================================================================
 Name        : dynamic_knapsack.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "knapsack.h"
#include "c_lib//heap.h"
#include "dynamic_knapsack.h"
#include "c_lib//tree_map.h"

static int compare_item(void *o1, void* o2){
	Item *i1 = o1;
	Item *i2 = o2;
	return i1->cost - i2->cost;
}

static void print_item(void *o){
	Item *i = o;
	printf("%d %d %d\n", i->id, i->cost, i->value);
}

void knapsack_sort(Item* items, int size, int (*compare)(void* o1, void* o2), void (*print)(void* o1)){
	Heap *heap = heap_constructor_print(print, compare);
	int counter;
	for(counter = 0; counter < size; counter++){
		heap_add(&items[counter], &items[counter], heap);
	}
	heap_buildMinHeap(heap);
	Item temp[size];
	for(counter = 0; counter < size; counter++){
		// ommmgg so i see the problem
		// so the pointers are pointing to a memory location,
		// but we are swapping and overwriting things before they have a
		// chance to be moved elsewhere ... basially we need a temp array...
		// its extra memory and an O(n) addition.
		// if u had a heap of integers wudnt need to arrays or anything like that...
		// DO NOT DELETE THIS COMMENT ... IMPORTANTE.
		temp[counter] = *(Item*)extractMinHeapMinimum(heap);
	}
	memcpy(items, temp, sizeof(Item)*size);
}

void quick_sort(void** array, int data_size, int array_size, int (*compare)(void*, void*)) {
	_quick_sort(0, array_size-1, array, data_size);
}

static void swap(int a, int b, void** array, int data_size){
	void* aptr = array[a];
	void* bptr = array[b];
	void* temp = malloc(data_size);
	memcpy(temp, aptr, data_size);
	memcpy(aptr, bptr, data_size);
	memcpy(bptr, temp, data_size);
}


int partition(int start, int end, void** array, int data_size, int (*compare)(void*, void*))
{
	int pivot = start;
	void* pivot_value = malloc(data_size);
	// copy the actual value to newly allocated data.
	memcpy(pivot_value, array[pivot], data_size);

	int pivot_counter;
	for (pivot_counter = start + 1; pivot_counter <= end; pivot_counter++)
	{
		if (compare(array[pivot_counter], pivot_value) > 0)
		{
			pivot++;
			swap(pivot_counter, pivot, array, data_size);
		}
	}
	swap(start, pivot, array, data_size);
	return pivot;
}

void _quick_sort(int start, int end, void** array, int data_size){
	if (start < end)
	{
		int pivot = partition(start, end, array, data_size, &compare_item);
		_quick_sort(start, pivot - 1, array, data_size);
		_quick_sort(pivot + 1, end, array, data_size);
	}
}

int max(int a, int b){
	if(a > b) return a;
	return b;
}

// no reason to not use a 2d array for this.
int dynamic_knapsack(Knapsack* knapsack)
{
   int i, w;
   int K[knapsack->item_count + 1][knapsack->max_cost + 1];
   for (i = 0; i < knapsack->item_count + 1; i++){
	   for (w = 0; w < knapsack->max_cost + 1; w++){
		   if (i==0 || w==0){
			   K[i][w] = 0;
		   }
		   else if(knapsack->items[i - 1].cost <= w){
			   // second parameter in max is K[i-1][w] and not K[i][w-1] because
			   // 'knapsack->items[i].value + K[i - 1][w - knapsack->items[i].cost]' will
			   // always be equal to or greater than K[i][w-1], but not always greater than
			   // K[i - 1][w]
			   // plut it is what we are adding to value ... '+ K[i - 1][w - knapsack->items[i].cost]'.
			   K[i][w] = max(knapsack->items[i-1].value + K[i - 1][w - knapsack->items[i-1].cost], K[i - 1][w]);
		   }
		   else{
			   K[i][w] = K[i - 1][w];
		   }
	   }
   }
   return K[knapsack->item_count][knapsack->max_cost];
}

int main()
{
    Knapsack* k = knapsack_constructor("knapsack16.input");
    // order does not matter, we care if it makes an optimal solution so far for w.
    // if it does it will be carried down.
    // works the way that shortest path between a and c and we go over path a to b then the way we got from a
    // to b must be the shortest way
    //knapsack_sort(k->items, k->item_count, &compare_item, &print_item);
    print_knapsack(k);
    printf("%d", dynamic_knapsack(k));
    return 0;
}

/*


int main()
{
	Knapsack* k = knapsack_constructor("knapsack128.input");
	knapsack_sort(k->items, k->item_count, &compare_item, &print_item);
	//print_knapsack(k);
	return 1;
}
*/
