#include <iostream>
#include "Hashtable.h"

using namespace std;

// This is the constructor. It sets the size of the hashtable.
// Its parameter is the size of the hashtable according to the user.
// It returns no value, but instead creates a new hashtable of size sz.
Hashtable::Hashtable(int sz) {
    size = sz;
    table.resize(sz);
}

// This function either adds a new pair to the hashtable, or updates
//      an existing pair's frequency.
// Its parameters are the string to be added and its respective frequency.
// It returns no value, but instead adds a new pair to the hashtable,
//      or updates an existing pair's frequency.
void Hashtable::put(string key, int frequency) {
    int accessPt = hash(key);
    
    if (get(key) == -1)
        table[accessPt].push_back(pair<string, int> (key, frequency));
    
    else {
        for (pair<string, int> & p : table[accessPt]) {
            if (p.first == key)
                p.second++;
        }
    }
}

// This function figures out the frequency of a word in the hashtable.
// The parameter is the word the user is searching for.
// It returns the words associated frequency if found, and returns
//      -1 if not found.
int Hashtable::get(string key) {
    int accessPt = hash(key);
    int value = -1;
    list<pair<string, int>> accessedList = table[accessPt];
    for (pair<string, int> & p : accessedList) {
        if (p.first == key)
            value = p.second;
    }
    
    return value;
}

// This function prints each list of the hashtable in a row.
// It has no parameters.
// It returns no value, but rather prints the hashtable by list.
void Hashtable::print(){
    for (int x = 0; x < table.size(); x++) {
        list<pair<string, int>> accessedList = table[x];
        cout << endl << "Table index " << x << " (" << accessedList.size() << "): ";
        for (pair<string, int> & p : accessedList)
            cout << p.first << ": " << p.second << ", ";
    }
}

// This function determines the hash value of a specific string.
// The parameter is the word to be hashed.
// It returns the hash value of s, the parameter word.
int Hashtable::hash(string s) {
    int value = 0;
    for (int x = 0; x < s.length(); x++)
        value += int(s.at(x));
    
    return (value % table.size());
}
