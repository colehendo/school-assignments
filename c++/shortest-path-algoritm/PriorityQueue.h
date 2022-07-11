#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <string>
#include <map>
#include <iostream>

using namespace std;

class PriorityQueue {
public:
    void insert(string s, int priority);
    void changePriority(string s, int newpriority);
    string extractMin();
    bool isEmpty() const;
    void print() const;
    
private:
    multimap<int, string> pq;
};


#endif
