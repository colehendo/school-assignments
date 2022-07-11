#include <iostream>
#include <fstream>
#include <sstream>
#include "Graph.h"
#include "PriorityQueue.h"

using namespace std;

vector<string> split(const string &s, char delim);
void dijkstra(Graph g, string start, string finish);

int main() {
    string filename;
    cout << "What filename do you want to read? ";
    cin >> filename;
    ifstream infile(filename);
    if (infile.fail()) {
        cout << "Could not open file." << endl;
        exit(0);
    }
    
    Graph g;
    string line;
    bool directed = true;
    while (getline(infile, line)) {
        vector<string> words = split(line, ' ');
        
        if (words[0] == "undirected")
            directed = false;
        
        if (words[0] == "vertex")
            g.addVertex(words[1]);
        
        if (words[0] == "edge")
            if (directed == true) {
                int newInt = stoi(words[3]);
                g.addEdge(words[1], words[2], newInt);
            }
            else {
                int newInt = stoi(words[3]);
                g.addEdge(words[1], words[2], newInt);
                g.addEdge(words[2], words[1], newInt);
            }
        }
        
        if (words[0] == "dijkstra") {
            set<string> vertSet = g.getVertices();
            cout << endl << endl << "Vertices: ";
            for (auto s : vertSet)
                cout << s << " ";
            
            cout << endl << "Edges:" << endl;
            g.print();
            cout << endl;
            dijkstra(g, words[1], words[2]);
        }
    }
    infile.close();
}

// This algorithm finds the shortest path between two vertices
// based on the weight of their connecting edges.
// The parameters are the graph, the starting vertex, and the ending vertex.
// It returns no value, but prints the shortest path and distance, along with
// the distance map, the previous map, and the values it changed.
void dijkstra(Graph g, string start, string finish) {
    map<string, int> dist;
    map<string, string> prev;
    vector<string> shortestPath;
    dist[start] = 0;
    set<string> vertices = g.getVertices();
    
    PriorityQueue Q;
    Q.insert(start, 0);
    
    set<string> newVerts = g.getVertices();
    for (const auto & v : newVerts) {
        if (v != start) {
            dist[v] = 99999999;
            prev[v] = "";
            Q.insert(v, dist[v]);
        }
    }
    
    while (!Q.isEmpty()) {
        string u = Q.extractMin();
        cout << "Visiting " << u << endl;
        if (u == finish)
            break;

        for (pair<string, int> & p : g.getEdges(u)) {
            int alt = dist[u] + g.getWeight(u, p.first);
            if (dist[u] == 99999999)
                alt = g.getWeight(u, p.first);
            if (alt < dist[p.first]) {
                int prevDist = dist[p.first];
                dist[p.first] = alt;
                prev[p.first] = u;
                Q.changePriority(p.first, alt);
                cout << "  Updating dist[" << p.first << "] from " << prevDist << " to " << alt << "." << endl;
            }
        }
    }
    
    for (pair<string, string> p : prev) {
        if (find(shortestPath.begin(), shortestPath.end(), p.second) != shortestPath.end())
            continue;
        else
            shortestPath.push_back(p.second);
    }
    shortestPath.push_back(finish);
    
    cout << endl << "Shortest path is:" << endl;
    for (auto s : shortestPath)
        cout << s << " ";
    cout << endl << "Distance is:" << endl << dist[finish] << endl;
    cout << endl << "Final dist map: " << endl;
    for (const auto & m : dist)
        cout << m.first << ": " << m.second << endl;
    
    cout << endl << "Final prev map: " << endl;
    for (const auto & m : prev)
        cout << m.first << ": " << m.second << endl;
}

vector<string> split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<string> elems;
    while (getline(ss, item, delim))
        elems.push_back(item);

    return elems;
}
