//============================================================================
// Name        : ExhaustiveGraphColor.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "GraphWrapper.h"
#include <ctime>
using namespace std;

void exhaustiveGraph(GraphWrapper *g, int t);

int main()
{
	string filenames[1] = {"color192-8"};
   for(int counter =0; counter < 1; counter++){
	   GraphWrapper* g = new GraphWrapper(filenames[counter]+".input");
	   exhaustiveGraph(g, 600);
	   g->printSolution(filenames[counter]+".output");
   }

}

void iterate(GraphWrapper *g, int n){
	int carry = 1;
	int index = 0;
	while(carry && index < n){
		if(g->getColor(index) == g->numColors-1) {
			g->setColor(index, 0);
			index++;
		} else {
			g->setColor(index, g->getColor(index)+1);
			carry = 0;
			return;
		}
	}
}

bool allUnselected(GraphWrapper *g, int n){
	for(int index = 0; index < n; index++){
		if(g->getColor(index)){
			return false;
		}
	}
	return true;
}

void setBestSolution(int* bestSolution, int n, GraphWrapper* g){
	for(int node = 0; node < n; node++){
		bestSolution[node] = g->getColor(node);
	}
}

void setGraph(int* bestSolution, int n, GraphWrapper* g){
	for(int node = 0; node < n; node++){
		g->setColor(node, bestSolution[node]);
	}
}

void exhaustiveGraph(GraphWrapper *g, int t){
	clock_t startTime = clock();
	int lowestValue = 1000;
	int n = g->numNodes;
	int* bestSolution = new int[n];
	do{
		if(lowestValue > g->getValue()){
			lowestValue = g->getValue();
			setBestSolution(bestSolution, n, g);
			cout << g->getValue() << endl;
		}
		iterate(g, n);
	}
	while(! allUnselected(g, n) && ((double)(clock()-startTime))/CLOCKS_PER_SEC <= t);
	setGraph(bestSolution, n, g);
	cout << "Time taken: \n" << ((double)(clock()-startTime))/CLOCKS_PER_SEC << endl;
}
