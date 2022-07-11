#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>

using namespace std;

class Graph {
public:
    void addVertex(string vertex);
    void addEdge(string v1, string v2, int weight);
    list<pair<string, int>> getEdges(string vertex) const;
    map<string, list<pair<string, int>>> returnMap() const;
    int getWeight(string v1, string v2) const;
    set<string> getVertices() const;
    void print() const;
    
private:
    set<string> vertices;
    map<string, list<pair<string, int>>> edges;
};


#endif
