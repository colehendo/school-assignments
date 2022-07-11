#ifndef game_hpp
#define game_hpp

#include "deck.hpp"
#include <stdio.h>
#include <vector>
#include <string>
using namespace std;

class Game {
public:
    Deck deck1;
    Deck deck2;
    string name1;
    string name2;
    int round = 1;
    
    //This is a simple constructor to allow the Game class to be used as a data type.
    //No parameters or return values.
    Game();
    
    //This is a constructor that stores both user's names.
    //It takes two strings as parameters.
    //It stores the two strings in name1 and name2 variables in the Game class.
    Game(string nName1, string nName2);
    
    //This function sets a player's deck to either the variable deck1 or deck2 based on the player's number.
    //This takes a player's number and their deck as parameters.
    //It does not return a value, but assigns the player's deck to either the variable deck1 or deck2.
    void setDeck(int player, const Deck & deck);
    
    //This function determines if the game is over by checking the size of each player's deck.
    //It takes no parameters, but returns true if either player's deck is size 0. If neither's is size 0, it returns false.
    bool isOver();
    
    //This function determines the name of the winner of the game by using the size of a deck.
    //If player1's deck is size 0, it returns player2's name.
    //If player2's deck is size 0, it returns player1's name.
    string getWinnerName();
    
    //This function plays the entire round.
    //It takes no parameters and returns no values, but adds or removes cards from player's decks and calls other previously described functions to play the game.
    void playRound();
};

#endif
