#ifndef card_hpp
#define card_hpp
#pragma once

#include <stdio.h>
#include <string>
using namespace std;

class Card {
public:
    char rank;
    char suit;
    
    //This is a simple constructor to allow the Card class to be used as a data type.
    //No parameters or return values.
    Card();
    
    //This is a constructor that takes a rank and suit of a card as chars.
    //It assigns the parameters to their respective variables in the Card class.
    Card(char nRank, char nSuit);
    
    //This is a function that takes the rank and suit of a card and converts them from individual chars to a string.
    //It returns this string.
    string toString();
    
    //This is a function that takes the char rank argument and converts its value to an integer using if and else if statements.
    //No parameters, and returns the int corresponding to the card's rank.
    int getValue();
    
    //This function takes two cards as arguments, one as the card type and one as the parameter, and compares them to determine a greater card value.
    //It returns true if the card entered as the card type has a greater card value than the one entered as a parameter.
    bool beats(Card & othercard);
    
    //This function takes two cards as arguments, one as the card type and one as the parameter, and compares them to determine if they are the same rank.
    //It returns true if both cards have the same rank.
    bool ties(Card & othercard);
};

#endif
