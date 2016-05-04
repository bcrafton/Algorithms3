// Project 1a: Solving knapsack using exhaustive search
//
#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <vector>
#include <time.h>
#include <math.h>

using namespace std;

#include "d_except.h"
#include "d_matrix.h"
#include "graph.h"
#include "knapsack.h"
#include "maxheap.h"

int compare(double &o1, double &o2){
	if(o1 == o2){return 0;}
	if(o1 > o2){return 1;}
	else{return -1;}
}

/*
void setKnapsack(knapsack &k, int option, int n){
	for(int bit = 0; bit < n; bit++){
		if(option%2){
			k.select(bit);
		} else {
			k.unSelect(bit);
		}
		option = option >> 1;
	}
}
*/

void iterate(knapsack &k, int n){
	int carry = 1;
	int index = 0;
	while(carry && index < n){
		if(k.isSelected(index)) {
			k.unSelect(index);
			index++;
		} else {
			k.select(index);
			carry = 0;
			return;
		}
	}
}

bool allUnselected(knapsack &k, int n){
	for(int index = 0; index < n; index++){
		if(k.isSelected(index)){
			return false;
		}
	}
	return true;
}

void setBestSolution(bool* bestSolution, int n, knapsack &k){
	for(int bit = 0; bit < n; bit++){
		bestSolution[bit] = k.isSelected(bit);
	}
}

void setKnapsack(bool* bestSolution, int n, knapsack &k){
	for(int bit = 0; bit < n; bit++){
		if(bestSolution[bit]){
			k.select(bit);
		}else{
			k.unSelect(bit);
		}
	}
}

// dont need item. key = value/cost and value = index in knapsack.
void greedyKnapsack(knapsack &k){
	MaxHeap<double, int>* heap = new MaxHeap<double, int>(&compare);
	for(int counter = 0; counter < k.getNumObjects(); counter++){
		double key = (double) k.getValue(counter) / k.getCost(counter);
		heap->add(key, counter);
	}
	while(! heap->empty()){
		int next = heap->extractMaxHeapMaximum();
		if(k.getCost(next) + k.getCost() <= k.getCostLimit()){
			k.select(next);
		}
	}
}

void exhaustiveKnapsack(knapsack &k, int t){
	clock_t startTime = clock();
	int highestValue = 0;
	int n = k.getNumObjects();
	bool* bestSolution = new bool[n];
	int counter = 0;
	do{
		if(highestValue < k.getValue() && k.getCost() <= k.getCostLimit()){
			highestValue = k.getValue();
			setBestSolution(bestSolution, n, k);
		}
		iterate(k, n);
		counter++;
	}
	while(! allUnselected(k, n) && ((double)(clock()-startTime))/CLOCKS_PER_SEC <= t);
	setKnapsack(bestSolution, n, k);
	cout << "Time taken: \n" << ((double)(clock()-startTime))/CLOCKS_PER_SEC;
}

void printSolution(knapsack &k, string filename)
// Prints out the solution.
{
	ofstream myfile;
	myfile.open (filename.c_str());
	myfile << "------------------------------------------------" << endl;

	myfile << "Total value: " << k.getValue() << endl;
	myfile << "Total cost: " << k.getCost() << endl << endl;

   // Print out objects in the solution
   for (int i = 0; i < k.getNumObjects(); i++)
      if (k.isSelected(i))
    	  myfile << i << "  " << k.getValue(i) << " " << k.getCost(i) << endl;

   myfile << endl;
   myfile.close();
}

/*
void exhaustiveKnapsack(knapsack &k, int t){
	int highestValue = 0;
	int bestSolution = 0;
	int n = (int)k.getNumObjects();
	long long int numberOfCombinations = pow (2, n);
	for(long long int option = 0; option < numberOfCombinations; option++){
		setKnapsack(k, option, n);
		if(highestValue < k.getValue() && k.getCost() <= k.getCostLimit()){
			highestValue = k.getValue();
			bestSolution = option;
		}
	}
	setKnapsack(k, bestSolution, n);
}
*/
int main()
{
	string filenames[12] = {"knapsack8", "knapsack12", "knapsack16",
			"knapsack20", "knapsack28", "knapsack32", "knapsack48",
			"knapsack64", "knapsack128", "knapsack256", "knapsack512",
			"knapsack1024"};

   ifstream fin;
   string fileName;

   for(int counter = 0; counter < 12; counter++){
	   fileName = filenames[counter] + ".input";

	   fin.open(fileName.c_str());
	   if (!fin)
	   {
	      cerr << "Cannot open " << fileName << endl;
	   }

	   try
	   {
	      knapsack k(fin);
	      greedyKnapsack(k);

	      cout << endl << "Best solution" << endl;
	      k.printSolution();
	      printSolution(k, filenames[counter]+".output");
	      fin.close();
	   }

	   catch (indexRangeError &ex)
	   {
	      cout << ex.what() << endl; //exit(1);
	   }
	   catch (rangeError &ex)
	   {
	      cout << ex.what() << endl; //exit(1);
	   }
   }

}

