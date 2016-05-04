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
#include "maxheap.h"
using namespace std;

void exhaustiveGraph(GraphWrapper *g, int t);
void greedyGraphColoring(GraphWrapper *g);
void greedyGraphColoringHeap(GraphWrapper *g);
void randomGraphColoring(GraphWrapper *g);
void twoOpt(GraphWrapper &g, int maxTime);
void threeOpt(GraphWrapper &g, int maxTime);

int main()
{
	string filenames[11] = {"color12-3", "color12-4", "color24-4",
			"color24-5", "color48-5", "color48-6", "color96-6",
			"color96-7", "color192-6", "color192-7", "color192-8"};
	for(int counter = 0; counter < 11; counter++){
		GraphWrapper* g = new GraphWrapper(filenames[counter]+".input");
		//greedyGraphColoring(g);
		randomGraphColoring(g);
		//twoOpt(*g, 600);
		threeOpt(*g, 600);
		//greedyGraphColoringHeap(g);
		g->printSolution(filenames[counter]+".output");
		cout << "finished: " + filenames[counter] +".input" << endl;
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

// the problem with this stradgy is that its not an undirected graph.
// can use an edge lookup, and check all edges that are directed or undirected to next
// this is not effiecient given the library limitations, but there is not reason why given a
// method that allows for adjacent directed edges (to or from) this could not be effeicient.

// this function is passed to graph wrappers breadth first traversal algorithm and executed on the front of the queue
// through each iteration of bft
void colorVertex(VertexKey next, GraphWrapper &g){
	bool* options = new bool[g.numColors];
	for(int counter = 0; counter < g.numColors; counter++){options[counter]=true;}
	pair<vertex_iterator, vertex_iterator> vItrRange = g.verticesIterator();
	for (vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr){
		VertexKey v = *vItr;
		if(g.getEdge(next, v).second || g.getEdge(v, next).second){
			options[g.getColor(v)] = false;
		}
	}
	for(int counter = 0; counter < g.numColors; counter++){
		if(options[counter]){
			g.setColor(next, counter);
			return;
		}
	}
	g.setColor(next, 0);
}

/**
 * Each time through trying each color combination for each node combination and seeing
 * if it is an improvement or not.
 */
void twoOpt(GraphWrapper &g){
	GraphWrapper champion(&g);
	bool change = true;
	while(change){
		change = false;
		int oldColorA;
		int oldColorB;
		for(int a = 0; a < g.numNodes; a++){
			for(int b = 0; b < g.numNodes; b++){
				oldColorA = g.getColor(a);
				oldColorB = g.getColor(b);
				for(int colora = 0; colora < g.numColors; colora++){
					for(int colorb = 0; colorb < g.numColors; colorb++){
						g.setColor(a,colora);
						g.setColor(b,colorb);
						if(g.getValue() < champion.getValue()){
							champion = g;
							change = true;
						}
					}
				}
				g.setColor(a,oldColorA);
				g.setColor(b,oldColorB);
			}
		}
		if(change){
			g = champion;
		}
	}
}
/**
 * I apologize for the ugly code
 * basically it is iterating through each 3 node combination and trying each possible
 * color combination for the three and seeing if the solution is an improvement.
 */
void threeOpt(GraphWrapper &g){
	GraphWrapper champion(&g);
	int oldColors[3];
	bool change = true;
	while(change){
		change = false;
		for(int a = 0; a < g.numNodes; a++){
			cout << a << endl;
			for(int b = 0; b < g.numNodes; b++){
				for(int c = 0; c < g.numNodes; c++){
					oldColors[0]= g.getColor(a);oldColors[1]= g.getColor(b);oldColors[2]= g.getColor(b);
					for(int colora = 0; colora < g.numColors; colora++){
						for(int colorb = 0; colorb < g.numColors; colorb++){
							for(int colorc = 0; colorc < g.numColors; colorc++){
								g.setColor(a,colora);g.setColor(b,colorb);g.setColor(b,colorc);
								if(g.getValue() < champion.getValue()){
									champion = g;
									change = true;
								}
							}
						}
					}
					g.setColor(a,oldColors[0]);g.setColor(b,oldColors[1]);g.setColor(c,oldColors[2]);
				}
			}
		}
		if(change){
			g = champion;
		}
	}
}



void randomColor(VertexKey next, GraphWrapper &g){
	g.setColor(next, rand()%g.numColors);
}

void greedyGraphColoring(GraphWrapper *g){
	g->BredthFirstTraversal(0, &colorVertex);
}

void randomGraphColoring(GraphWrapper *g){
	g->BredthFirstTraversal(0, &randomColor);
}

// so what order shud the nodes be pulled in? high to low degree?
int compare(int &o1, int &o2){
	if(o1 == o2){return 0;}
	else if(o1 > o2){return 1;}
	else{return -1;}
}

void greedyGraphColoringHeap(GraphWrapper *g){
	MaxHeap<int, VertexKey>* heap = new MaxHeap<int, VertexKey>(&compare);
	pair<vertex_iterator, vertex_iterator> vItrRange = g->verticesIterator();
	for (vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr){
		int degree = g->edgeCount(*vItr);
		heap->add(degree, *vItr);
	}
	heap->buildMaxHeap();
	while(! heap->empty()){
		VertexKey next = heap->extractMaxHeapMaximum();
		colorVertex(next, *g);
	}
}



