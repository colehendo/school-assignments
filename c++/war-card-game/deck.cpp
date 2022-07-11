#include "deck.hpp"
#include <stdio.h>
#include <vector>
#include <string>
using namespace std;

//This is a simple constructor to allow the Deck class to be used as a data type.
//No parameters or return values.
Deck::Deck() {}

//This function takes the vector cards and stores the first value in currentCard. It then deletes the first value and its slot.
//The function returns the variable currentCard.
Card Deck::dealFromTop() {
    Card currentCard = cards.front();
    cards.erase(cards.begin());
    return currentCard;
}

//This function adds a variable to the end of the vector cards.
//It takes a card as a variable and does not return a value.
void Deck::addToBottom(const Card & card) {
    cards.push_back(card);
}

//This function determines the size of the deck.
//It returns the deck size as an int.
int Deck::size() {
    int deckSize = cards.size();
    return deckSize;
}

//This function determines if the deck is empty.
//It returns true if the deck size is 0. It returns false if the size is not 0.
bool Deck::isEmpty() {
    if (cards.size() == 0)
        return true;
    return false;
}

//This function converts the vector cards into a string of each element of the vector with a space between each and put in brackets.
//It returns the string in brackets.
string Deck::toDeck() {
    string deckString = "[";
    for (int i = 0; i < cards.size(); i++) {
        string x = cards[i].toString();
        deckString += x;
        deckString += " ";
    }
    deckString += "]";
    return deckString;
}
