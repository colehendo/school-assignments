#include "Graph.h"
#include <iostream>

using namespace std;

// Adds a new vertex to the graph.
// The parameter is the vertex to be added.
// There is no return value. It adds a vertex to the set
// in the .h file.
void Graph::addVertex(string vertex) {
    vertices.insert(vertex);
}

// Adds a new edge to the graph between vertices v1 and v2 with the specified weight.
// The parameters are the vertecies the edge is being added between, and the weight of the edge.
// It returns no value, but instead adds the edge to the map.
void Graph::addEdge(string v1, string v2, int weight) {
    pair<string, int> newPair(v2, weight);
    list<pair<string, int>> newEdge;
    newEdge.push_back(newPair);
    bool found = false;
    for (pair<string, list<pair<string, int>>> p : edges) {
        if (p.first == v1) {
            found = true;
            edges[v1].push_back(newPair);
        }
    }

    if (found == false)
        edges[v1] = newEdge;
}

// Return the entire set of vertices in the graph.
// It has no parameters.
// It returns the entire set of vertices in the graph.
set<string> Graph::getVertices() const {
    return vertices;
}

map<string, list<pair<string, int>>> Graph::returnMap() const {
    return edges;
}

// Returns the edges that are connected to the vertex specified.
// The parameter is the vertex that the edges are connected to.
// It returns a list of edges and vertices connected to the parameter vertex.
list<pair<string, int>> Graph::getEdges(string vertex) const {
    list<pair<string, int>> returnedList;
    
    for (pair<string, list<pair<string, int>>> p : edges) {
        if (vertex == p.first)
            returnedList = p.second;
    }
    return returnedList;
};

// Returns the weight of the edge between vertices v1 and v2.
// The parameters are the vertexes connected by the edge to be returned.
// It returns the weight value of the edge connecting the two parameter vertexes.
int Graph::getWeight(string v1, string v2) const {
    list<pair<string, int>> foundList;
    int toReturn = -1;
    
    for (pair<string, list<pair<string, int>>> p : edges) {
        if (v1 == p.first)
            foundList = p.second;
    }
    
    for (pair<string, int> & p : foundList) {
        if (v2 == p.first)
            toReturn = p.second;
    }
    
    return toReturn;
}

// Prints a textual representation of the graph.
// No parameters.
// It returns no value, but prints a representation of the graph.
void Graph::print() const {
    for (pair<string, list<pair<string, int>>> p : edges) {
        string currVertex = p.first;
        list<pair<string, int>> currList = p.second;
        
        for (pair<string, int> & p : currList)
            cout << currVertex << " -> " << p.first << ": " << p.second << endl;
    }

}
