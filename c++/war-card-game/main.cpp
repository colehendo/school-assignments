#include <iostream>
#include "card.hpp"
#include "deck.hpp"
#include "game.hpp"
#include <fstream>
using namespace std;

//The main function sets various variables to read through the file, and calls functions from other files to play the game.
int main() {
    Game game;
    ifstream infile;
    string fileName;
    string firstPlayerName;
    string secondPlayerName;
    string firstCard;
    string secondCard;
    char firstCardFirstChar;
    char firstCardSecondChar;
    char secondCardFirstChar;
    char secondCardSecondChar;
    Card card1;
    Card card2;
    Deck firstPlayerDeck;
    Deck secondPlayerDeck;
    
    infile.open("./text.txt");
    if (!infile.is_open()) {
        cout << "Unable to open file" << endl;
        exit(1);
    }
    
    else {
        getline(infile, firstPlayerName);
        getline(infile, secondPlayerName);
        Game(firstPlayerName, secondPlayerName);
        while (getline(infile, firstCard)) {
            getline(infile, secondCard);
            firstCardFirstChar = firstCard[0];
            firstCardSecondChar = firstCard[1];
            secondCardFirstChar = secondCard[0];
            secondCardSecondChar = secondCard[1];
            card1 = Card(firstCardFirstChar, firstCardSecondChar);
            card2 = Card(secondCardFirstChar, secondCardSecondChar);
            firstPlayerDeck.addToBottom(card1);
            secondPlayerDeck.addToBottom(card2);
        }
        game.deck1 = firstPlayerDeck;
        game.deck2 = secondPlayerDeck;
        game.name1 = firstPlayerName;
        game.name2 = secondPlayerName;
        game.playRound();
    }
    
    return 0;
}
