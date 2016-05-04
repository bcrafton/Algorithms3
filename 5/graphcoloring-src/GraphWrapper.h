#include <iostream>
#include <list>
#include <queue>
#include <vector>
#include <stack>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>

using namespace std;
using namespace boost;

struct VertexProperties;
struct EdgeProperties;
typedef adjacency_list<vecS, vecS, bidirectionalS, VertexProperties, EdgeProperties> Graph;
typedef Graph::vertex_descriptor VertexKey;
typedef Graph::edge_descriptor EdgeKey;
typedef Graph::vertex_iterator vertex_iterator;
typedef Graph::edge_iterator edge_iterator;
typedef Graph::adjacency_iterator adjacent_iterator;
#define UNCOLORED -1;

struct VertexProperties
{
   int color;
   Graph::vertex_descriptor pred;
   bool visited;
   bool marked;
   int weight;
};

// Create a struct to hold properties for each edge
struct EdgeProperties
{
   int weight;
   bool visited;
   bool marked;
};

class GraphWrapper{
private:
	Graph graph;
public:
	int numNodes;
	int numEdges;
	int numColors;
	GraphWrapper(string filename);
	GraphWrapper(GraphWrapper* g);
	pair<EdgeKey, bool> addEdge(VertexKey u, VertexKey v);
	VertexKey addVertex();
	int numberOfEdges();
	int numberOfVertices();
	VertexKey edge_target(EdgeKey e);
	VertexKey edge_source(EdgeKey e);
	void removeEdge(VertexKey u, VertexKey v);
	pair<EdgeKey, bool> getEdge(VertexKey u, VertexKey v);
	void DepthFirstTraversal(VertexKey start);
	void BredthFirstTraversal(VertexKey start);
	void BredthFirstTraversal(VertexKey start, void (*foo)(VertexKey next, GraphWrapper &g));
	void clearVisited();
	void setColor(VertexKey v, int color);
	int getColor(VertexKey v);
	int getValue();
	void printSolution(string filename);
	bool isColored(VertexKey v);
	int edgeCount(VertexKey v);

	pair<vertex_iterator, vertex_iterator> verticesIterator();
	pair<edge_iterator, edge_iterator> edgeIterator();
	pair<adjacent_iterator, adjacent_iterator> adjacentVertices(VertexKey v);
};

GraphWrapper::GraphWrapper(string filename){
	ifstream fin;
	fin.open(filename.c_str());
	if (!fin){cerr << "Cannot open " << filename << endl;}
	int node1;
	int node2;
	fin >> numColors;
	fin >> numNodes;
	fin >> numEdges;
	for(int vertexCounter = 0; vertexCounter < numNodes; vertexCounter++){
		this->addVertex();
		graph[vertexCounter].color = UNCOLORED;
	}
	for(int edgeCounter = 0; edgeCounter < numEdges; edgeCounter++){
		fin >> node1;
		fin >> node2;
		this->addEdge(node1, node2);
	}
}
GraphWrapper::GraphWrapper(GraphWrapper* g){
	this->numColors = numColors;
	this->numEdges = numEdges;
	this->numNodes = numNodes;
	for(int vertexCounter = 0; vertexCounter < numNodes; vertexCounter++){
		this->addVertex();
		graph[vertexCounter].color = g->getColor(vertexCounter);
	}
	pair<edge_iterator, edge_iterator> edgeItrRange = g->edgeIterator();
	for (edge_iterator edgeItr = edgeItrRange.first; edgeItr != edgeItrRange.second; ++edgeItr){
		this->addEdge(g->edge_source(*edgeItr), g->edge_target(*edgeItr));
	}
}
pair<EdgeKey, bool> GraphWrapper::addEdge(VertexKey u, VertexKey v){
	return add_edge(u, v, this->graph);
}
VertexKey GraphWrapper::addVertex(){
	return add_vertex(this->graph);
}
int GraphWrapper::numberOfEdges(){
	return num_edges(this->graph);
}
int GraphWrapper::numberOfVertices(){
	return num_vertices(this->graph);
}
VertexKey GraphWrapper::edge_target(EdgeKey e){
	return target(e, this->graph);
}
VertexKey GraphWrapper::edge_source(EdgeKey e){
	return source(e, this->graph);
}
void GraphWrapper::removeEdge(VertexKey u, VertexKey v){
	remove_edge(u, v, this->graph);
}
// named the pair edge originally, syntax error because calling the edge function as well.
pair<EdgeKey, bool> GraphWrapper::getEdge(VertexKey u, VertexKey v){
	return edge(u, v, this->graph);
}
pair<vertex_iterator, vertex_iterator> GraphWrapper::verticesIterator(){
	return vertices(this->graph);
}
pair<edge_iterator, edge_iterator> GraphWrapper::edgeIterator(){
	return edges(this->graph);
}
pair<adjacent_iterator, adjacent_iterator> GraphWrapper::adjacentVertices(VertexKey v){
	return adjacent_vertices(v, graph);
}

void GraphWrapper::DepthFirstTraversal(VertexKey start){
	stack<VertexKey> s;
	s.push(start);
	graph[start].visited = true;
	while(!s.empty()){
		VertexKey top = s.top();
		s.pop();
		pair<adjacent_iterator, adjacent_iterator> adjacentItrRange = adjacentVertices(top);
		for (adjacent_iterator adjacentItr = adjacentItrRange.first; adjacentItr != adjacentItrRange.second; ++adjacentItr)
		{
			VertexKey next = *adjacentItr;
			if(! graph[next].visited){
				graph[next].visited = true;
				s.push(next);
			}
		}
		cout << top << " ";
	}
	cout << "\n";
}

void GraphWrapper::clearVisited(){
	pair<vertex_iterator, vertex_iterator> vItrRange = verticesIterator();
	for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr){
		graph[*vItr].visited = false;
	}
}

void GraphWrapper::BredthFirstTraversal(VertexKey start){
	queue<VertexKey> q;
	q.push(start);
	graph[start].visited = true;
	while(!q.empty()){
		VertexKey front = q.front();
		q.pop();
		pair<adjacent_iterator, adjacent_iterator> adjacentItrRange = adjacentVertices(front);
		for (adjacent_iterator adjacentItr = adjacentItrRange.first; adjacentItr != adjacentItrRange.second; ++adjacentItr)
		{
			VertexKey next = *adjacentItr;
			if(! graph[next].visited){
				graph[next].visited = true;
				q.push(next);
			}
		}
		cout << front << " ";
	}
	cout << "\n";
}

void GraphWrapper::BredthFirstTraversal(VertexKey start, void (*foo)(VertexKey next, GraphWrapper &g)){
	queue<VertexKey> q;
	q.push(start);
	graph[start].visited = true;
	while(!q.empty()){
		VertexKey front = q.front();
		(*foo)(front, *this);
		q.pop();
		pair<adjacent_iterator, adjacent_iterator> adjacentItrRange = adjacentVertices(front);
		for (adjacent_iterator adjacentItr = adjacentItrRange.first; adjacentItr != adjacentItrRange.second; ++adjacentItr)
		{
			VertexKey next = *adjacentItr;
			if(! graph[next].visited){
				graph[next].visited = true;
				q.push(next);
			}
		}
	}
}

void GraphWrapper::setColor(VertexKey v, int color){
	this->graph[v].color = color;
}

int GraphWrapper::getColor(VertexKey v){
	return this->graph[v].color;
}

int GraphWrapper::getValue(){
	int value = 0;
	pair<vertex_iterator, vertex_iterator> vertices = verticesIterator();
	for (vertex_iterator vItr= vertices.first; vItr != vertices.second; ++vItr){
		VertexKey next = *vItr;
		pair<adjacent_iterator, adjacent_iterator> adjacentIterator = adjacentVertices(next);
		for (adjacent_iterator aItr= adjacentIterator.first; aItr != adjacentIterator.second; ++aItr){
			VertexKey adjacentVertex = *aItr;
			if(graph[next].color == graph[adjacentVertex].color){
				value++;
			}
		}
	}
	return value;
}
void GraphWrapper::printSolution(string filename){
	ofstream myfile;
	myfile.open (filename.c_str());

	int vertexCount = numNodes;
	myfile << "------------------------------------------------" << endl;
	myfile << "Total conflicts: " << this->getValue() << endl;
	for(int counter = 0; counter < vertexCount; counter++){
		myfile << counter << ": " << graph[counter].color << endl;
	}
	myfile.close();
}
bool GraphWrapper::isColored(VertexKey v){
	return graph[v].color != -1;
}
int GraphWrapper::edgeCount(VertexKey v){
	int edgeCount = 0;
	pair<vertex_iterator, vertex_iterator> vItrRange = verticesIterator();
	for (vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr){
		VertexKey next = *vItr;
		if(getEdge(next, v).second || getEdge(v, next).second){
			edgeCount++;
		}
	}
	return edgeCount;
}
