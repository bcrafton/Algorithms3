/*
 * maxheap.h
 *
 *  Created on: May 22, 2015
 *      Author: brian
 */

#ifndef MAXHEAP_H_
#define MAXHEAP_H_

#include <vector>
#include <iostream>
using namespace std;

static int NULL_INDEX = -1;

template <typename T, typename U>
class HeapNode{
public:
	T key;
	U value;
	HeapNode(T key, U value){
		this->key = key;
		this->value = value;
	}
};
template <typename T, typename U>
class MaxHeap{
private:
	int size;
	int (*heap_compare_function)(T t1, T t2);
	vector<HeapNode<T,U>*> v;
	int parent(int current_index);
	int right(int current_index);
	int left(int current_index);
	void swap(int index1, int index2);
	int compare(HeapNode<T,U>* h1, HeapNode<T,U>* h2);
public:
	MaxHeap(int (*heap_compare_function)(T, T));
	void maxHeapify(int current_index);
	void add(T key, U value);
	void buildMaxHeap();
	void sort_inc();
	int getSize();
	U getMaxHeapMaximum();
	U extractMaxHeapMaximum();
	void maxHeapIncreaseKey(int current_index);
	void maxHeapInsert(T key ,U value);
	int indexOf(U value);
	U get(int index);
	bool empty();
};

template <typename T, typename U>
MaxHeap<T,U>::MaxHeap(int (*heap_compare_function)(T, T)){
	this->size = 0;
	this->heap_compare_function = heap_compare_function;
}

template <typename T, typename U>
int MaxHeap<T,U>::parent(int current_index){
	return current_index/2;
}
template <typename T, typename U>
int MaxHeap<T,U>::right(int current_index){
	return 2*(current_index+1);
}
template <typename T, typename U>
int MaxHeap<T,U>::left(int current_index){
	return 2*current_index+1;
}
template <typename T, typename U>
void MaxHeap<T,U>::maxHeapify(int current_index){
	int left_child_index;
	int right_child_index;
	int largest_index = NULL_INDEX;
	int heapSize = size;

	while(largest_index != current_index){
		left_child_index = left(current_index);
		right_child_index = right(current_index);

		HeapNode<T,U>* current = v[current_index];
		HeapNode<T,U>* left_child = v[left_child_index];
		HeapNode<T,U>* right_child = v[right_child_index];

		if(left_child_index <= heapSize-1 && compare(left_child, current) > 0 ) {
			largest_index = left_child_index;
		} else {
			largest_index = current_index;
		}

		HeapNode<T,U>* largest = v[largest_index];

		if(right_child_index <= heapSize-1 && compare(right_child, largest) > 0 ) {
			largest_index = right_child_index;
		}

		if(largest_index != current_index) {
			swap(current_index, largest_index);
			current_index = largest_index;
			largest_index = NULL_INDEX;
		}
	}
}

template <typename T, typename U>
void MaxHeap<T,U>::add(T key, U value){
	HeapNode<T,U>* node = new HeapNode<T,U>(key, value);
	v.push_back(node);
	size++;
}

template <typename T, typename U>
void MaxHeap<T,U>::buildMaxHeap(){
	int index_counter;
	for(index_counter = (size)/2-1; index_counter >= 0; index_counter--)
	{
		maxHeapify(index_counter);
	}
}

template <typename T, typename U>
void MaxHeap<T,U>::sort_inc(){
	buildMaxHeap();

	int counter;
	int heapSize = size;

	for(counter = heapSize - 1; counter >= 1; counter--){
		swap(0, counter);
		size--;
		maxHeapify(0);
	}
	size = heapSize;
}

template <typename T, typename U>
U MaxHeap<T,U>::getMaxHeapMaximum(){
	HeapNode<T,U>* node = v[0];
	return node->value;
}

template <typename T, typename U>
U MaxHeap<T,U>::extractMaxHeapMaximum(){
	HeapNode<T,U>* top = v[0];
	v[0] = v[size-1];
	v[size-1] = NULL;
	size--;
	if(size > 1)
	maxHeapify(0);
	return top->value;
}

template <typename T, typename U>
int MaxHeap<T,U>::getSize(){
	return size;
}

template <typename T, typename U>
void MaxHeap<T,U>::maxHeapIncreaseKey(int current_index){
	while(current_index > 0 && compare(v[parent(current_index)], v[current_index]) < 0){
		swap(current_index, parent(current_index));
      	current_index = parent(current_index);
	}
}

template <typename T, typename U>
void MaxHeap<T,U>::maxHeapInsert(T key, U value){
	add(key, value);
	cout << "F";
	maxHeapIncreaseKey(size-1);
}

template <typename T, typename U>
int MaxHeap<T,U>::indexOf(U value){
	int counter;
	for(counter = 0; counter < v.size; counter++){
		HeapNode<T,U>* node = v[counter];
		if(value == node->value){
			return counter;
		}
	}
	return -1;
}

template <typename T, typename U>
U MaxHeap<T,U>::get(int index){
	return v[index]->value;
}

template <typename T, typename U>
void MaxHeap<T,U>::swap(int index1, int index2){
	HeapNode<T,U>* temp = v[index1];
	v[index1] = v[index2];
	v[index2] = temp;
}
template <typename T, typename U>
int MaxHeap<T,U>::compare(HeapNode<T,U>* h1, HeapNode<T,U>* h2){
	return (*(heap_compare_function))(h1->key, h2->key);
}
template <typename T, typename U>
bool MaxHeap<T,U>::empty(){
	return size == 0;
}
#endif /* MAXHEAP_H_ */
