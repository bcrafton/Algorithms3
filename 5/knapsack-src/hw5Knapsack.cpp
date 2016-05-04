
// Project 4: Solving knapsack using branch and bound
//

#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <vector>
#include <time.h>

#include "d_except.h"
#include "d_matrix.h"
#include "graph.h"
#include "maxheap.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;

class knapsack
{
   public:
      knapsack();
      knapsack(ifstream &fin);
      knapsack(const knapsack &k);
      knapsack &operator=(const knapsack &k);

      int getCost(int) const;
      int getValue(int) const;
      int getIndex(int) const;

      int getCurrentValue() const;
      int getCurrentCost() const;

      bool isSelected(int) const;

      void setCost(int, int);
      void setValue(int, int);
      void setIndex(int, int);

      void select(int);
      void unSelect(int);
      void unSelectAll();

      int getNumObjects() const;
      int getCostBound() const;
      void printSolution(vector<bool> &solution) const;
      void sortObjects();

      void setNum(int);
      int getNum() const;
      float bound() const;

   private:
      int numObjects;
      int costBound;
      vector<int> value;
      vector<int> cost;
      vector<int> index;
      vector<bool> selected;
      int num;
      int currentValue;
      int currentCost;
};

knapsack::knapsack()
// Create a blank knapsack instance
{
   numObjects = 0;
   costBound = 0;

   value.resize(0);
   cost.resize(0);
   index.resize(0);
   selected.resize(0);

   setNum(0);
   currentValue = 0;
   currentCost = 0;
}

knapsack::knapsack(ifstream &fin)
// Construct a new knapsack instance using the data in fin.  Mark all
// objects as unselected.
{
   int n, b, j, v, c;

   fin >> n;  // read the number of objects
   fin >> b;  // read the cost bound

   numObjects = n;
   costBound = b;

   value.resize(n);
   cost.resize(n);
   index.resize(n);
   selected.resize(n);

   for (int i = 0; i < n; i++)
   {
      fin >> j >> v >> c;
      setValue(j,v);
      setCost(j,c);
      setIndex(i,i);
      unSelect(j);
   }

   setNum(0);
   currentValue = 0;
   currentCost = 0;
}

knapsack::knapsack(const knapsack &k)
// Knapsack copy constructor
{
   numObjects = k.getNumObjects();
   costBound = k.getCostBound();

   value.resize(numObjects);
   cost.resize(numObjects);
   index.resize(numObjects);
   selected.resize(numObjects);

   for (int i = 0; i < numObjects; i++)
   {
      setValue(i,k.getValue(i));
      setCost(i,k.getCost(i));
      setIndex(i,k.getIndex(i));

      if (k.isSelected(i))
	 select(i);
      else
	 unSelect(i);
   }

   setNum(k.getNum());
   currentValue = k.getCurrentValue();
   currentCost = k.getCurrentCost();
}

knapsack &knapsack::operator=(const knapsack &k)
// Knapsack assignment operator
{
   numObjects = k.getNumObjects();
   costBound = k.getCostBound();

   value.resize(numObjects);
   cost.resize(numObjects);
   index.resize(numObjects);
   selected.resize(numObjects);

   for (int i = 0; i < numObjects; i++)
   {
      setValue(i,k.getValue(i));
      setCost(i,k.getCost(i));
      setIndex(i,k.getIndex(i));

      if (k.isSelected(i))
	 select(i);
      else
	 unSelect(i);
   }

   setNum(k.getNum());
   currentValue = k.getCurrentValue();
   currentCost = k.getCurrentCost();

   return *this;
}

int knapsack::getNumObjects() const
{
   return numObjects;
}

int knapsack::getCostBound() const
{
   return costBound;
}


int knapsack::getValue(int i) const
// Return the value of the ith object.
{
   if (i < 0 || i >= getNumObjects())
      throw rangeError("Bad value in knapsack::getValue");

   return value[i];
}

int knapsack::getCost(int i) const
// Return the cost of the ith object.
{
   if (i < 0 || i >= getNumObjects())
   {
      throw rangeError("Bad value in knapsack::getCost");
   }

   return cost[i];
}

int knapsack::getCurrentValue() const
// Return the total value of the selected objects
{
   return currentValue;
}

int knapsack::getCurrentCost() const
// Return the total cost of the selected objects
{
   return currentCost;
}

int knapsack::getIndex(int i) const
// Return the index of the ith object.
{
   if (i < 0 || i >= getNumObjects())
      throw rangeError("Bad value in knapsack::getIndex");

   return index[i];
}

bool knapsack::isSelected(int i) const
// Returns true if object i has been selected, and false
// otherwise.
{
   if (i < 0 || i >= getNumObjects())
      throw rangeError("Bad value in knapsack::isSelected");

   return selected[i];
}

void knapsack::setValue(int i, int n)
// Set the ith object's value to n.
{
   if (i < 0 || i >= getNumObjects())
      throw rangeError("Bad value in knapsack::setValue");

   value[i] = n;
}

void knapsack::setCost(int i, int n)
// Set the ith object's cost to n.
{
   if (i < 0 || i >= getNumObjects())
      throw rangeError("Bad value in knapsack::getCost");

   cost[i] = n;
}

void knapsack::setIndex(int i, int n)
// Set the ith object's index to n.
{
   if (i < 0 || i >= getNumObjects())
      throw rangeError("Bad value in knapsack::setIndex");

   index[i] = n;
}

void knapsack::select(int i)
// Select the ith object and update current value and cost
{
   if (i < 0 || i >= getNumObjects())
      throw rangeError("Bad value in knapsack::select");

   if (!selected[i])
   {
      currentValue = currentValue + getValue(i);
      currentCost = currentCost + getCost(i);
   }
   selected[i] = true;
}

void knapsack::unSelect(int i)
// Unselect the ith object and update current value and cost
{
   if (i < 0 || i >= getNumObjects())
      throw rangeError("Bad value in knapsack::unSelect");

   if (selected[i])
   {
      currentValue = currentValue - getValue(i);
      currentCost = currentCost - getCost(i);
   }
   selected[i] = false;
}

void knapsack::unSelectAll()
// Unselect all objects
{
   for (int i = 0; i < getNumObjects(); i++)
      unSelect(i);
}


void knapsack::setNum(int n)
// Update the number of objects that have been considered.
{
   num = n;
}

int knapsack::getNum() const
// Return the number of objects that have been considered.
{
   return num;
}

ostream &operator<<(ostream &ostr, const knapsack &k)
// Print all information about the knapsack.
{
   cout << "------------------------------------------------" << endl;
   cout << "Num objects: " << k.getNumObjects() << " Cost bound: " << k.getCostBound() << endl;

   int totalValue = 0;
   int totalCost = 0;

   cout << "Total value: " << k.getCurrentValue() << endl;
   cout << "Total cost: " << k.getCurrentCost() << endl << endl;

   for (int i = 0; i < k.getNumObjects(); i++)
      if (k.isSelected(i))
	 cout << k.getIndex(i) << "  " << k.getValue(i) << " " << k.getCost(i) << endl;

   cout << endl;

   return ostr;
}

void knapsack::printSolution(vector<bool> &solution) const
// Prints out the solution.
{
   cout << "------------------------------------------------" << endl;

   int totalValue = 0;
   int totalCost = 0;

   // Add up values for objects in the solution

   for (int i = 0; i < getNumObjects(); i++)
   {
      if (solution[i])
      {
	 totalValue += getValue(i);
	 totalCost += getCost(i);
      }
   }

   cout << "Total value: " << totalValue << endl;
   cout << "Total cost: " << totalCost << endl << endl;

   // Print out objects in the solution
   for (int i = 0; i < getNumObjects(); i++)
      if (solution[i])
	 cout << getIndex(i) << "  " << getValue(i) << " " << getCost(i) << endl;

   cout << endl;
}

ostream &operator<<(ostream &ostr, vector<bool> v)
// Overloaded output operator for vectors.
{
   for (int i = 0; i < v.size(); i++)
      cout << v[i] << endl;

   return ostr;
}

void knapsack::sortObjects()
// Sort the objects by decreasing ratio of value/cost using insertion
// sort.
{
   int value, cost, index;
   int j;

   cout << "Sorting objects" << endl;

   for (int i = 1; i < getNumObjects(); i++)
   {
      value = getValue(i);
      cost = getCost(i);
      index = getIndex(i);

      j = i-1;
      while (j >= 0 && (float) getValue(j) / getCost(j) < (float) value/cost)
      {
         setValue(j+1,getValue(j));
         setCost(j+1,getCost(j));
         setIndex(j+1,getIndex(j));
	 j = j-1;
      }
      setValue(j+1,value);
      setCost(j+1,cost);
      setIndex(j+1,index);
   }
}

float knapsack::bound() const
// Return an upper bound on the optimal value of objects that can fit
// in the knapsack, given that decisions have already been made about
// the first num objects.
//
// The bound is equal to the value of the objects already selected,
// plus the solution to the fractional knapsack problem for the empty
// part of the knapsack.  Fits as many objects with the largest
// value/cost ratio as can fit in the empty part of the knapsack.
{
	float value = getCurrentValue();
	float cost = getCurrentCost();
	int count = 0;
	while(cost < getCostBound() && getNum()+count < getNumObjects()){
		if(cost + getCost(getNum()+count) < getCostBound()){
			cost = cost + getCost(getNum()+count);
			value = value + getValue(getNum()+count);
		} else {
			value = value + ((getCostBound()-cost)/getCostBound())*getValue(getNum()+count);
			cost = getCostBound();
		}
		count++;
	}
	return value;
}

int compare(float &o1, float &o2){
	if(o1 == o2){return 0;}
	if(o1 > o2){return 1;}
	else{return -1;}
}

double greedyKnapsack(knapsack &k){
	MaxHeap<float, int>* heap = new MaxHeap<float, int>(&compare);
	for(int counter = 0; counter < k.getNumObjects(); counter++){
		double key = (double) k.getValue(counter) / k.getCost(counter);
		heap->add(key, counter);
	}
	while(! heap->empty()){
		int next = heap->extractMaxHeapMaximum();
		if(k.getCost(next) + k.getCurrentCost() <= k.getCostBound()){
			k.select(next);
		}
	}
	return k.getCurrentValue();
}

void branchAndBound(knapsack &k, int maxTime)
// Implement a Branch and Bound search for an optimal solution.
// Searches for the best decision for objects n and greater, as long
// as more than maxTime seconds have not elapsed.  Tries
// to keep or not keep object n, and then iterates to n+1.  Stores
// the best solution found so far in bestSolution.
{
	deque<knapsack> problem;

   clock_t startTime = clock();


   knapsack bestSolution(k);
   knapsack pessimisticBound(k);
   greedyKnapsack(pessimisticBound);
   float bestValue = pessimisticBound.getCurrentValue();

   // Initially, decisions have not been made about any objects,
   // so set num = 0.
   k.setNum(0);

   // Add the empty knapsack subproblem to the list
   problem.push_back(k);
   // Branch and Bound search goes here
   while(! problem.empty() && ((float)clock()-(float)startTime)/CLOCKS_PER_SEC < maxTime){
	   knapsack next = problem.front();problem.pop_front();
	   if(next.getCurrentValue() > bestSolution.getCurrentValue()){
		   bestSolution = next;
		   if(bestSolution.getCurrentValue() > bestValue)
			   bestValue = bestSolution.getCurrentValue();
	   }
	   knapsack k1(next);
	   k1.select(k1.getNum());
	   k1.setNum(k1.getNum() + 1);
	   if(k1.getCurrentCost() <= k1.getCostBound() && k1.bound() > bestValue && k1.getNum() < k1.getNumObjects()){
		   problem.push_back(k1);
	   }
	   knapsack k2(next);
	   k2.setNum(k2.getNum() + 1);
	   if(k2.bound() > bestValue && k2.getNum() < k2.getNumObjects()){
		   problem.push_back(k2);
	   }
    }
   cout << "Best value found: " << bestSolution.getCurrentValue() << endl;
   cout << bestSolution;
}

void printSolution(knapsack &k, string filename)
// Prints out the solution.
{
	ofstream myfile;
	myfile.open (filename.c_str());
	myfile << "------------------------------------------------" << endl;

	myfile << "Total value: " << k.getCurrentValue() << endl;
	myfile << "Total cost: " << k.getCurrentCost() << endl << endl;

   // Print out objects in the solution
   for (int i = 0; i < k.getNumObjects(); i++)
      if (k.isSelected(i))
    	  myfile << i << "  " << k.getValue(i) << " " << k.getCost(i) << endl;

   myfile << endl;
   myfile.close();
}

void randomKnapsack(knapsack &k){
	srand (time(NULL));
	for(int counter = 0; counter < k.getNumObjects(); counter++){
		if(rand()%2 && k.getCurrentCost() + k.getCost(counter) < k.getCostBound()){
			k.select(counter);
		}
	}
}
/**
 * taking each combination of 3 knapsack elements and trying each different way of
 * selecting them. Then choosing the best improvement over the set.
 * I really wish I saw a better way of writing this, it is hard to read.
 */
void threeOpt(knapsack &k, int maxTime){
	bool change=true;
	knapsack champion(k);
	bool oldStates[3];
	float bestValue = 0;
	clock_t startTime = clock();
	while(change && ((float)clock()-(float)startTime)/CLOCKS_PER_SEC < maxTime){
		change = false;
		bestValue = k.getCurrentValue();
		for(int a = 0; a < k.getNumObjects(); a++){
			if(((float)clock()-(float)startTime)/CLOCKS_PER_SEC > maxTime)break;
			for(int b = 0; b < k.getNumObjects(); b++){
				for(int c = 0; c < k.getNumObjects(); c++){
					oldStates[0] = k.isSelected(a); oldStates[1] = k.isSelected(b); oldStates[2]=k.isSelected(c);
					for(int option= 0; option < 8; option++){
						option%2 ? k.select(a) : k.unSelect(a);
						(option>>1)%2 ? k.select(b) : k.unSelect(b);
						(option>>2)%2 ? k.select(c) : k.unSelect(c);
						if(k.getCurrentValue() > bestValue && k.getCurrentCost() <= k.getCostBound()){
							bestValue = k.getCurrentValue();
							change = true;
							champion = k;
						}
						oldStates[0] ? k.select(a) : k.unSelect(a);
						oldStates[1] ? k.select(b) : k.unSelect(b);
						oldStates[2] ? k.select(c) : k.unSelect(c);
					}
				}
			}
		}
		if(change){
			k = champion;
		}
	}
}
/**
 * taking each combination of 2 knapsack elements and trying each different way of
 * selecting them. Then choosing the best improvement over the set.
 */
void twoOpt(knapsack &k, int maxTime){
	float maxChange=1;
	float change=0;
	int besti, bestj;
	clock_t startTime = clock();
	while(maxChange > 0 && ((float)clock()-(float)startTime)/CLOCKS_PER_SEC < maxTime){
		maxChange = 0;
		for(int i = 0; i < k.getNumObjects(); i++){
			for(int j = 0; j < k.getNumObjects(); j++){
				if(k.isSelected(i) && !k.isSelected(j)){
					change = k.getValue(j) - k.getValue(i);
					if(change > maxChange && k.getCost(j) - k.getCost(i) + k.getCurrentCost() <= k.getCostBound()){
						maxChange = change;
						besti = i;
						bestj = j;
					}
				}
			}
		}
		if(maxChange > 0){
			k.unSelect(besti);
			k.select(bestj);
		}
	}
}
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
		  //greedyKnapsack(k);
		  randomKnapsack(k);
		  threeOpt(k, 600);
		  //twoOpt(k, 600);
		  printSolution(k, filenames[counter]+".output");
		  fin.close();
		  cout << "finished: " + filenames[counter] +".input" << endl;
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
