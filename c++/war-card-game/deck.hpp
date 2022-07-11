#ifndef deck_hpp
#define deck_hpp

#include "card.hpp"
#include <stdio.h>
#include <vector>
#include <string>
using namespace std;

class Deck {
public:
    vector<Card> cards;
    
    //This is a simple constructor to allow the Deck class to be used as a data type.
    //No parameters or return values.
    Deck();
    
    //This function takes a vector and stores the first value in a variable. It then deletes the first value and its slot.
    //The function returns that variable.
    Card dealFromTop();
    
    //This function adds a variable to the end of a vector.
    //It takes a card as a variable and does not return a value.
    void addToBottom(const Card & card);
    
    //This function determines the size of the deck.
    //It returns the deck size as an int.
    int size();
    
    //This function determines if the deck is empty.
    //It returns true if the deck size is 0. It returns false if the size is not 0.
    bool isEmpty();
    
    //This function converts the vector cards into a string of each element of the vector with a space between each and put in brackets.
    //It returns the string in brackets.
    string toDeck();
};

#endif
