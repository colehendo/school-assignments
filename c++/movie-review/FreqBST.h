#ifndef BST_H
#define BST_H

#include <iostream>

using namespace std;

struct WordNode {
    string word;
    double frequency;
    double totalScore;
    WordNode *left = nullptr;
    WordNode *right = nullptr;
};

class FreqBST {
public:
    ~FreqBST();

    void addScore(string word, int score);

    void remove(string word);
    double getAverageScore(string word) const;

    void printFrequencyTable(WordNode *curr) const;

    void printPreorder(WordNode *curr) const;
    void printInorder(WordNode *curr) const;
    void printPostorder(WordNode *curr) const;

    int countWords(WordNode *curr) const;
    int height(WordNode *curr) const;

    WordNode accessRoot();

private:
    WordNode *root = nullptr;
};

#endif
