#include <iostream>
#include <fstream>
#include <sstream>
#include "Hashtable.h"

using namespace std;

vector<string> split(const string &s, char delim);
void quicksort(vector<pair<string, int>> &x, int first, int last);
int binarySearch(vector<pair<string, int>> x, string word);

int main() {
    cout << "How big should the hashtable be? ";
    int htsize;
    cin >> htsize;
    
    Hashtable freqHt(htsize);
    vector<pair<string, int>> freqVec;
    
    string filename, line;
    cout << "What filename do you want to read? ";
    cin >> filename;
    ifstream infile(filename);
    if (infile.fail()) {
        cout << "Could not open file." << endl;
        exit(0);
    }
    
    while (getline(infile, line)) {
        vector<string> words = split(line, ' ');
        for (int x = 0; x < words.size(); x++) {
            string word = words[x];
            
            freqHt.put(word, 1);
            
            bool insideFreqVec = false;
            for (pair<string, int> & p : freqVec) {
                if (word == p.first) {
                    p.second++;
                    insideFreqVec = true;
                    break;
                }
            }
            
            if (insideFreqVec == false)
                freqVec.push_back(pair<string, int>(word, 1));
        }
    }
    infile.close();
    
    cout << "\nHash table:";
    freqHt.print();
    
    cout << endl << "\nUnsorted vector: " << endl;
    
    int x = 0;
    for (pair<string, int> & p : freqVec) {
        cout << "Vector index " << x << ": " << p.first << ": " << p.second << endl;
        x++;
    }
    
    quicksort(freqVec, 0, (freqVec.size() - 1));

    cout << endl << "Sorted vector: " << endl;
    
    x = 0;
    for (pair<string, int> & p : freqVec) {
        cout << "Vector index " << x << ": " << p.first << ": " << p.second << endl;
        x++;
    }
    
    string word;
    while (true) {
        cout << "\nEnter a word to search for: ";
        cin >> word;
        if (word == "quit")
            break;
        
        int freq = freqHt.get(word);
        if (freq == -1)
            freq = 0;
        int freq2 = binarySearch(freqVec, word);
        cout << "Hashtable frequency = " << freq << endl;
        cout << "Vector frequency = " << freq2 << endl;
    }
    
    return 0;
}

// This function swaps two pairs in a vector.
// The parameters are the two pairs to be swapped.
// It returns nothing, but rather swaps the pairs
//      through the reference operator.
void swap2(pair<string, int> &x, pair<string, int> &y) {
    pair<string, int> temp = x;
    x = y;
    y = temp;
}

// This function partially sorts a vector around a pivot element.
// The parameters are the vector to be sorted, its first
//      element, and its last element.
// It returns the pivot element, which is the only confirmed sorted
//      element of the vector once the function runs.
int split2(vector<pair<string, int>> &x, int first, int last) {
    pair<string, int> pivot = x[first];
    int left = first;
    int right = last;
    while (left < right) {
        while (pivot.first < x[right].first)
            right--;
        while (left < right && (x[left].first < pivot.first || x[left].first == pivot.first))
            left++;
        if (left < right)
            swap2(x[left], x[right]);
    }
    int pos = right;
    x[first] = x[pos];
    x[pos] = pivot;
    return pos;
}

// This function sorts an unsorted vector in-place.
// The parameters are the unsorted vector, the first position
//      in the vector, and the last position in the vector.
// It returns nothing, but rather sorts the vector in the parameter
//      through the reference operator.
void quicksort(vector<pair<string, int>> &x, int first, int last){
    int pos;
    if (first < last) {
        pos = split2(x, first, last);
        quicksort(x, first, pos - 1);
        quicksort(x, pos + 1, last);
    }
}

// This function searches a sorted vector for a specific word.
// The parameters are the vector to be searched and the word
//      being searched for.
// It returns the frequency associated with the word being
//      searched for if the word is found. Otherwise, it
//      returns 0.
int binarySearch(vector<pair<string, int>> x, string word){
    int left = 0;
    int mid = 0;
    int right = x.size();
    int value = 0;
    while (left < right) {
        mid = left + ((right - left) / 2);
        if (word > x[mid].first)
            left = mid + 1;
        else if (word < x[mid].first)
            right = mid;
        else {
            value = x[mid].second;
            break;
        }
    }
    return value;
}

vector<string> split(const string &s, char delim)
{
    stringstream ss(s);  // this requires #include <sstream>
    string item;
    vector<string> elems;
    while (getline(ss, item, delim))
        elems.push_back(item);
=
    return elems;
}
