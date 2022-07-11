#include "card.hpp"
#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;

//This is a simple constructor to allow the Card class to be used as a data type.
//No parameters or return values.
Card::Card() {}

//This is a constructor that takes a rank and suit of a card as chars.
//It assigns the parameters to their respective variables in the Card class.
Card::Card(char nRank, char nSuit) {
    rank = nRank;
    suit = nSuit;
}

//This is a function that takes the rank and suit of a card and converts them from individual chars to a string.
//It returns this string.
string Card::toString() {
    string newRank = string(1, rank);
    string newSuit = string(1, suit);
    string newCard = newRank + newSuit;
    return newCard;
}

//This is a function that takes the char rank argument and converts its value to an integer using if and else if statements.
//No parameters, and returns the int corresponding to the card's rank.
int Card::getValue() {
    if (rank == '2')
        return 2;
    else if (rank == '3')
        return 3;
    else if (rank == '4')
        return 4;
    else if (rank == '5')
        return 5;
    else if (rank == '6')
        return 6;
    else if (rank == '7')
        return 7;
    else if (rank == '8')
        return 8;
    else if (rank == '9')
        return 9;
    else if (rank == 'T')
        return 10;
    else if (rank == 'J')
        return 11;
    else if (rank == 'Q')
        return 12;
    else if (rank == 'K')
        return 13;
    else if (rank == 'A')
        return 14;
    return 0;
}

//This function takes two cards as arguments, one as the card type and one as the parameter, and compares them to determine a greater card value.
//It returns true if the card entered as the card type has a greater card value than the one entered as a parameter.
bool Card::beats(Card & othercard) {
    int firstRank = getValue();
    if (firstRank > othercard.getValue())
        return true;
    return false;
}

//This function takes two cards as arguments, one as the card type and one as the parameter, and compares them to determine if they are the same rank.
//It returns true if both cards have the same rank.
bool Card::ties(Card & othercard) {
    int firstRank = getValue();
    if (firstRank == othercard.getValue())
        return true;
    return false;
}
