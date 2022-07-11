#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <string>
#include <map>
#include <iostream>

using namespace std;

class PriorityQueue {
public:
    void insert(string s, int priority);    // add a new item into the priority queue
    void changePriority(string s, int newpriority); // change the priority of an item already in the queue
    string extractMin();    // return the item with the minimum priority
    bool isEmpty() const;   // check if the queue is empty
    void print() const;     // print the elements & their priorities in the queue
    
private:
    multimap<int, string> pq;
};


#endif
