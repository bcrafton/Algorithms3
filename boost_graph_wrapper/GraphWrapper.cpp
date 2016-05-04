#include <iostream>
#include <list>
#include <queue>
#include <vector>
#include <stack>
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

struct VertexProperties
{
   pair<int,int> cell; // maze cell (x,y) value
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
	GraphWrapper();
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
	void clearVisited();

	pair<vertex_iterator, vertex_iterator> verticesIterator();
	pair<edge_iterator, edge_iterator> edgeIterator();
	pair<adjacent_iterator, adjacent_iterator> adjacentVertices(VertexKey v);
};
GraphWrapper::GraphWrapper(){
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
		cout << top;
	}
	cout << "\n";
}

void GraphWrapper::clearVisited(){
	pair<vertex_iterator, vertex_iterator> vertexItrRange = verticesIterator();
	for (vertex_iterator adjacentItr = vertexItrRange.first; adjacentItr != vertexItrRange.second; ++adjacentItr)
	{
		graph[*adjacentItr].visited = false;
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
		cout << front;
	}
	cout << "\n";
}

int main()
{
	GraphWrapper *g = new GraphWrapper();
	g->addVertex();
	g->addVertex();
	g->addVertex();
	g->addVertex();
	g->addEdge(0,1);
	g->addEdge(1,2);
	g->addEdge(2,3);
	g->addEdge(3,4);
	g->addEdge(1,0);
	g->addEdge(2,1);
	g->addEdge(3,2);
	g->addEdge(4,3);

	g->DepthFirstTraversal(2);
	g->clearVisited();
	g->BredthFirstTraversal(2);
}
