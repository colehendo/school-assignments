#include "PriorityQueue.h"

using namespace std;

void PriorityQueue::insert(string s, int priority) {
    pq.insert(pair<int, string>(priority, s));
}

void PriorityQueue::changePriority(string s, int newpriority) {
    for (auto it = pq.begin(); it != pq.end(); it++) {
        if (it->second == s) {
            pq.erase(it);
            pq.insert({newpriority, s});
            break;
        }
    }
}

string PriorityQueue::extractMin() {
    auto it = pq.begin();
    string minstring = it->second;
    pq.erase(it);
    return minstring;
    
}

bool PriorityQueue::isEmpty() const {
    return pq.size() == 0;
}

void PriorityQueue::print() const {
    for (pair<int, string> p : pq)
        cout << p.first << " " << p.second << endl;
}
