#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <vector>
#include <list>

using namespace std;


class Hashtable {
public:
    // This is the constructor. It sets the size of the hashtable.
    // Its parameter is the size of the hashtable according to the user.
    // It returns no value, but instead creates a new hashtable of size sz.
    Hashtable(int sz);
    
    // This function either adds a new pair to the hashtable, or updates
    //      an existing pair's frequency.
    // Its parameters are the string to be added and its respective frequency.
    // It returns no value, but instead adds a new pair to the hashtable,
    //      or updates an existing pair's frequency.
    void put(string key, int frequency);
    
    // This function figures out the frequency of a word in the hashtable.
    // The parameter is the word the user is searching for.
    // It returns the words associated frequency if found, and returns
    //      -1 if not found.
    int get(string key);
    
    // This function prints each list of the hashtable in a row.
    // It has no parameters.
    // It returns no value, but rather prints the hashtable by list.
    void print();
    
private:
    int size;
    vector<list<pair<string, int>>> table;
    
    // This function determines the hash value of a specific string.
    // The parameter is the word to be hashed.
    // It returns the hash value of s, the parameter word.
    int hash(string s);
};


#endif
