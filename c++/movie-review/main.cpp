#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <set>
#include <vector>
#include <string>
#include "FreqBST.h"

vector<string> split(const string &s, char delim);

using namespace std;

int main() {
    const bool PRINT_TREES = true;

    FreqBST wordFreqs;

    string filename, line;
    cout << "What filename do you want to read? ";
    cin >> filename;
    ifstream infile(filename);
    if (infile.fail()) {
        cout << "Could not open file." << endl;
        exit(0);
    }

    while (getline(infile, line)) {
        vector<string> review = split(line, ' ');
        int size = review.size();
        int score = stoi(review[0]);
        for(int x = 1; x < size; x++){
            wordFreqs.addScore(review[x], score);
        }
    }
    infile.close();

    WordNode root = wordFreqs.accessRoot();

    cout << "Number of words is: " << wordFreqs.countWords(&root) << endl;
    cout << "Height of the tree is: " << wordFreqs.height(&root) << endl;
    if (PRINT_TREES) {
        cout << "Preorder traversal: ";
        wordFreqs.printPreorder(&root);
        cout << "\nInorder traversal: ";
        wordFreqs.printInorder(&root);
        cout << "\nPostorder traversal: ";
        wordFreqs.printPostorder(&root);
        cout << "\nFrequency table: " << endl;
        wordFreqs.printFrequencyTable(&root);
        cout << endl;
    }
    
    infile.open("stopwords.txt");
    if (infile.fail()) {
        cout << "Could not open file." << endl;
        exit(0);
    }
    
    set<string> stopwords;

    while (getline(infile, line))
        stopwords.insert(line);
    
    // This iterates through the set of stop words
    // removing them from the BST
    set<string>::iterator position;
    for (position = stopwords.begin(); position != stopwords.end(); position++)
        wordFreqs.remove(*position);
    
    cout << "After removing stopwords: " << endl;
    cout << "Number of words is: " << wordFreqs.countWords(&root) << endl;
    cout << "Height of the tree is: " << wordFreqs.height(&root) << endl;
    if (PRINT_TREES) {
        cout << "Preorder traversal: ";
        wordFreqs.printPreorder(&root);
        cout << "\nInorder traversal: ";
        wordFreqs.printInorder(&root);
        cout << "\nPostorder traversal: ";
        wordFreqs.printPostorder(&root);
        cout << "\nFrequency table: " << endl;
        wordFreqs.printFrequencyTable(&root);
        cout << endl;
    }
    
    double totalScore = 0;
    double totalCount = 0;
    int linePos = 0;
    string word = "";
    
    // This determines the score of a movie review entered by the user
    // by traversing the BST and averaging the scores of all the words entered
    while (true) {
        string line;
        cout << "\nEnter a new review to analyze: ";
        getline(cin, line);
        if (line == "quit") break;
        if (line == "") getline(cin, line); // getline is weird
    
        int skip = 0;
        
        for (int x = linePos; x < line.length(); x++) {
            if (line[x] == ' ') {
                linePos++;
                for (position = stopwords.begin(); position != stopwords.end(); position++) {
                    if (*position == word) {
                        cout << "Skipping " << word << endl;
                        skip++;
                    }
                }
                
                if (skip == 1) {
                    word = "";
                    continue;
                }
                
                else {
                    totalCount++;
                    double avgSet = wordFreqs.getAverageScore(word);
                    cout << "The average setiment of " << word << " is " << avgSet << endl;
                    totalScore += avgSet;
                    word = "";
                }
            }
            
            else {
                skip = 0;
                word += line[x];
                linePos++;
            }
        }

        totalCount++;
        double avgSet = wordFreqs.getAverageScore(word);
        cout << "The average setiment of " << word << " is " << avgSet << endl;
        totalScore += avgSet;
        word = "";
        
        cout << "The setiment score for this review is " << totalScore/totalCount << endl;
        
        totalScore = 0;
        totalCount = 0;
        linePos = 0;
    }
    return 0;
}

vector<string> split(const string &s, char delim) {
    stringstream ss(s);  // this requires #include <sstream>
    string item;
    vector<string> elems;
    while (getline(ss, item, delim))
        elems.push_back(item);

    return elems;
}
