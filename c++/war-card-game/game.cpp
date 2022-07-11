#include "game.hpp"
#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

//This is a simple constructor to allow the Game class to be used as a data type.
//No parameters or return values.
Game::Game() {}

//This is a constructor that stores both user's names.
//It takes two strings as parameters.
//It stores the two strings in name1 and name2 variables in the Game class.
Game::Game(string nName1, string nName2) {
    name1 = nName1;
    name2 = nName2;
}

//This function sets a player's deck to either the variable deck1 or deck2 based on the player's number.
//This takes a player's number and their deck as parameters.
//It does not return a value, but assigns the player's deck to either the variable deck1 or deck2.
void Game::setDeck(int player, const Deck & deck) {
    if (player == 1)
        Deck deck1 = deck;
    else
        Deck deck2 = deck;
}

//This function determines if the game is over by checking the size of each player's deck.
//It takes no parameters, but returns true if either player's deck is size 0. If neither's is size 0, it returns false.
bool Game::isOver() {
    if (deck1.size() == 0)
        return true;
    else if (deck2.size() == 0)
        return true;]
    return false;
}

//This function determines the name of the winner of the game by using the size of a deck.
//If player1's deck is size 0, it returns player2's name.
//If player2's deck is size 0, it returns player1's name.
string Game::getWinnerName() {
    if (deck1.size() == 0)
        return name2;
    else if (deck2.size() == 0)
        return name1;
    return "Game is not over.";
}

//This function plays the entire round.
//It takes no parameters and returns no values, but adds or removes cards from player's decks and calls other previously described functions to play the game.
void Game::playRound() {
    while (isOver() == false){
        cout << endl << "Round " << round << endl;
        cout << name1 << "'s deck: " << deck1.toDeck() << endl;
        cout << name2 << "'s deck: " << deck2.toDeck() << endl;
        
        Card card1 = deck1.dealFromTop();
        Card card2 = deck2.dealFromTop();
        
        cout << name1 << " plays " << card1.toString() << endl;
        cout << name2 << " plays " << card2.toString() << endl;
        
        if (card1.beats(card2) == true) {
            cout << name1 << " wins the round!" << endl;
            deck1.addToBottom(card1);
            deck1.addToBottom(card2);
            cout << name1 << "'s deck: " << deck1.toDeck() << endl;
            cout << name2 << "'s deck: " << deck2.toDeck() << endl;
        }
        
        else if (card2.beats(card1) == true) {
            cout << name2 << " wins the round!" << endl;
            deck2.addToBottom(card2);
            deck2.addToBottom(card1);
            cout << name1 << "'s deck: " << deck1.toDeck() << endl;
            cout << name2 << "'s deck: " << deck2.toDeck() << endl;
        }
    
        while (card1.ties(card2) == true) {
            Card card3 = deck1.dealFromTop();
            Card card4 = deck1.dealFromTop();
            Card card5 = deck2.dealFromTop();
            Card card6 = deck2.dealFromTop();
            Card card7;
            Card card8;
            cout << name1 << " plays " << card3.toString() << " face down and " << card4.toString() << " face up." << endl;
            cout << name2 << " plays " << card5.toString() << " face down and " << card6.toString() << " face up." << endl;
            card7 = card1;
            card8 = card2;
            card1 = card4;
            card2 = card6;
            
            if (card4.beats(card6) == true) {
                cout << name1 << " wins the war!" << endl;
                deck1.addToBottom(card7);
                deck1.addToBottom(card3);
                deck1.addToBottom(card1);
                deck1.addToBottom(card8);
                deck1.addToBottom(card5);
                deck1.addToBottom(card2);
                cout << name1 << "'s deck: " << deck1.toDeck() << endl;
                cout << name2 << "'s deck: " << deck2.toDeck() << endl;
                break;
            }
            
            else if (card6.beats(card4) == true) {
                cout << name2 << " wins the war!" << endl;
                deck2.addToBottom(card8);
                deck2.addToBottom(card5);
                deck2.addToBottom(card2);
                deck2.addToBottom(card7);
                deck2.addToBottom(card3);
                deck2.addToBottom(card4);
                cout << name1 << "'s deck: " << deck1.toDeck() << endl;
                cout << name2 << "'s deck: " << deck2.toDeck() << endl;
                break;
            }
            
            else {
                Card card9 = deck1.dealFromTop();
                Card card10 = deck1.dealFromTop();
                Card card11 = deck2.dealFromTop();
                Card card12 = deck2.dealFromTop();
                Card card13;
                Card card14;
                cout << name1 << " plays " << card9.toString() << " face down and " << card10.toString() << " face up." << endl;
                cout << name1 << " plays " << card11.toString() << " face down and " << card12.toString() << " face up." << endl;
                card13 = card4;
                card14 = card6;
                card4 = card10;
                card6 = card12;
                
                if (card10.beats(card12) == true) {
                    cout << name1 << " wins the war!" << endl;
                    deck1.addToBottom(card7);
                    deck1.addToBottom(card3);
                    deck1.addToBottom(card1);
                    deck1.addToBottom(card9);
                    deck1.addToBottom(card4);
                    deck1.addToBottom(card8);
                    deck1.addToBottom(card5);
                    deck1.addToBottom(card2);
                    deck1.addToBottom(card11);
                    deck1.addToBottom(card6);
                    cout << name1 << "'s deck: " << deck1.toDeck() << endl;
                    cout << name2 << "'s deck: " << deck2.toDeck() << endl;
                    break;
                }
                
                else if (card12.beats(card10) == true) {
                    cout << name2 << " wins the war!" << endl;
                    deck2.addToBottom(card8);
                    deck2.addToBottom(card5);
                    deck2.addToBottom(card2);
                    deck2.addToBottom(card11);
                    deck2.addToBottom(card6);
                    deck2.addToBottom(card7);
                    deck2.addToBottom(card3);
                    deck2.addToBottom(card4);
                    deck2.addToBottom(card9);
                    deck2.addToBottom(card4);
                    cout << name1 << "'s deck: " << deck1.toDeck() << endl;
                    cout << name2 << "'s deck: " << deck2.toDeck() << endl;
                    break;
                }
            
                else {
                    Card card15 = deck1.dealFromTop();
                    Card card16 = deck1.dealFromTop();
                    Card card17 = deck2.dealFromTop();
                    Card card18 = deck2.dealFromTop();
                    Card card19;
                    Card card20;
                    cout << name1 << " plays " << card15.toString() << " face down and " << card16.toString() << " face up." << endl;
                    cout << name1 << " plays " << card17.toString() << " face down and " << card18.toString() << " face up." << endl;
                    card19 = card10;
                    card20 = card12;
                    card10 = card16;
                    card12 = card18;
                    
                    if (card16.beats(card18) == true) {
                        cout << name1 << " wins the war!" << endl;
                        deck1.addToBottom(card7);
                        deck1.addToBottom(card3);
                        deck1.addToBottom(card1);
                        deck1.addToBottom(card9);
                        deck1.addToBottom(card4);
                        deck1.addToBottom(card15);
                        deck1.addToBottom(card10);
                        deck1.addToBottom(card8);
                        deck1.addToBottom(card5);
                        deck1.addToBottom(card2);
                        deck1.addToBottom(card11);
                        deck1.addToBottom(card6);
                        deck1.addToBottom(card17);
                        deck1.addToBottom(card12);
                        cout << name1 << "'s deck: " << deck1.toDeck() << endl;
                        cout << name2 << "'s deck: " << deck2.toDeck() << endl;
                        break;
                    }
                    
                    else if (card18.beats(card16) == true) {
                        cout << name2 << " wins the war!" << endl;
                        deck2.addToBottom(card8);
                        deck2.addToBottom(card5);
                        deck2.addToBottom(card2);
                        deck2.addToBottom(card11);
                        deck2.addToBottom(card6);
                        deck2.addToBottom(card17);
                        deck2.addToBottom(card12);
                        deck2.addToBottom(card7);
                        deck2.addToBottom(card3);
                        deck2.addToBottom(card4);
                        deck2.addToBottom(card9);
                        deck2.addToBottom(card4);
                        deck2.addToBottom(card15);
                        deck2.addToBottom(card10);
                        cout << name1 << "'s deck: " << deck1.toDeck() << endl;
                        cout << name2 << "'s deck: " << deck2.toDeck() << endl;
                        break;
                    }
                
                    else {
                        Card card21 = deck1.dealFromTop();
                        Card card22 = deck1.dealFromTop();
                        Card card23 = deck2.dealFromTop();
                        Card card24 = deck2.dealFromTop();
                        Card card25;
                        Card card26;
                        cout << name1 << " plays " << card21.toString() << " face down and " << card22.toString() << " face up." << endl;
                        cout << name1 << " plays " << card23.toString() << " face down and " << card24.toString() << " face up." << endl;
                        card25 = card16;
                        card26 = card18;
                        card16 = card22;
                        card18 = card24;
                        
                        if (card22.beats(card24) == true) {
                            cout << name1 << " wins the war!" << endl;
                            deck1.addToBottom(card7);
                            deck1.addToBottom(card3);
                            deck1.addToBottom(card1);
                            deck1.addToBottom(card9);
                            deck1.addToBottom(card4);
                            deck1.addToBottom(card15);
                            deck1.addToBottom(card10);
                            deck1.addToBottom(card21);
                            deck1.addToBottom(card16);
                            deck1.addToBottom(card8);
                            deck1.addToBottom(card5);
                            deck1.addToBottom(card2);
                            deck1.addToBottom(card11);
                            deck1.addToBottom(card6);
                            deck1.addToBottom(card17);
                            deck1.addToBottom(card12);
                            deck1.addToBottom(card23);
                            deck1.addToBottom(card18);
                            cout << name1 << "'s deck: " << deck1.toDeck() << endl;
                            cout << name2 << "'s deck: " << deck2.toDeck() << endl;
                            break;
                        }
                        
                        else if (card24.beats(card22) == true) {
                            cout << name2 << " wins the war!" << endl;
                            deck2.addToBottom(card8);
                            deck2.addToBottom(card5);
                            deck2.addToBottom(card2);
                            deck2.addToBottom(card11);
                            deck2.addToBottom(card6);
                            deck2.addToBottom(card17);
                            deck2.addToBottom(card12);
                            deck2.addToBottom(card23);
                            deck2.addToBottom(card18);
                            deck2.addToBottom(card7);
                            deck2.addToBottom(card3);
                            deck2.addToBottom(card4);
                            deck2.addToBottom(card9);
                            deck2.addToBottom(card4);
                            deck2.addToBottom(card15);
                            deck2.addToBottom(card10);
                            deck2.addToBottom(card21);
                            deck2.addToBottom(card16);
                            cout << name1 << "'s deck: " << deck1.toDeck() << endl;
                            cout << name2 << "'s deck: " << deck2.toDeck() << endl;
                            break;
                        }
                    
                        else {
                            Card card27 = deck1.dealFromTop();
                            Card card28 = deck1.dealFromTop();
                            Card card29 = deck2.dealFromTop();
                            Card card30 = deck2.dealFromTop();
                            Card card31;
                            Card card32;
                            cout << name1 << " plays " << card27.toString() << " face down and " << card28.toString() << " face up." << endl;
                            cout << name1 << " plays " << card29.toString() << " face down and " << card30.toString() << " face up." << endl;
                            card31 = card22;
                            card32 = card24;
                            card22 = card28;
                            card24 = card30;
                            
                            if (card28.beats(card30) == true) {
                                cout << name1 << " wins the war!" << endl;
                                deck1.addToBottom(card7);
                                deck1.addToBottom(card3);
                                deck1.addToBottom(card1);
                                deck1.addToBottom(card9);
                                deck1.addToBottom(card4);
                                deck1.addToBottom(card15);
                                deck1.addToBottom(card10);
                                deck1.addToBottom(card21);
                                deck1.addToBottom(card16);
                                deck1.addToBottom(card27);
                                deck1.addToBottom(card22);
                                deck1.addToBottom(card8);
                                deck1.addToBottom(card5);
                                deck1.addToBottom(card2);
                                deck1.addToBottom(card11);
                                deck1.addToBottom(card6);
                                deck1.addToBottom(card17);
                                deck1.addToBottom(card12);
                                deck1.addToBottom(card23);
                                deck1.addToBottom(card24);
                                deck1.addToBottom(card29);
                                deck1.addToBottom(card24);
                                cout << name1 << "'s deck: " << deck1.toDeck() << endl;
                                cout << name2 << "'s deck: " << deck2.toDeck() << endl;
                                break;
                            }
                            
                            else if (card30.beats(card28) == true) {
                                cout << name2 << " wins the war!" << endl;
                                deck2.addToBottom(card8);
                                deck2.addToBottom(card5);
                                deck2.addToBottom(card2);
                                deck2.addToBottom(card11);
                                deck2.addToBottom(card6);
                                deck2.addToBottom(card17);
                                deck2.addToBottom(card12);
                                deck2.addToBottom(card23);
                                deck2.addToBottom(card18);
                                deck2.addToBottom(card29);
                                deck2.addToBottom(card24);
                                deck2.addToBottom(card7);
                                deck2.addToBottom(card3);
                                deck2.addToBottom(card4);
                                deck2.addToBottom(card9);
                                deck2.addToBottom(card4);
                                deck2.addToBottom(card15);
                                deck2.addToBottom(card10);
                                deck2.addToBottom(card27);
                                deck2.addToBottom(card22);
                                cout << name1 << "'s deck: " << deck1.toDeck() << endl;
                                cout << name2 << "'s deck: " << deck2.toDeck() << endl;
                                break;
                            }
                        
                            else {
                                Card card33 = deck1.dealFromTop();
                                Card card34 = deck1.dealFromTop();
                                Card card35 = deck2.dealFromTop();
                                Card card36 = deck2.dealFromTop();
                                Card card37;
                                Card card38;
                                cout << name1 << " plays " << card33.toString() << " face down and " << card34.toString() << " face up." << endl;
                                cout << name1 << " plays " << card35.toString() << " face down and " << card36.toString() << " face up." << endl;
                                card37 = card28;
                                card38 = card30;
                                card28 = card34;
                                card30 = card36;
                                
                                if (card1.beats(card2) == true) {
                                    cout << name1 << " wins the war!" << endl;
                                    deck1.addToBottom(card7);
                                    deck1.addToBottom(card3);
                                    deck1.addToBottom(card1);
                                    deck1.addToBottom(card9);
                                    deck1.addToBottom(card4);
                                    deck1.addToBottom(card15);
                                    deck1.addToBottom(card10);
                                    deck1.addToBottom(card21);
                                    deck1.addToBottom(card16);
                                    deck1.addToBottom(card27);
                                    deck1.addToBottom(card22);
                                    deck1.addToBottom(card33);
                                    deck1.addToBottom(card28);
                                    deck1.addToBottom(card8);
                                    deck1.addToBottom(card5);
                                    deck1.addToBottom(card2);
                                    deck1.addToBottom(card11);
                                    deck1.addToBottom(card6);
                                    deck1.addToBottom(card17);
                                    deck1.addToBottom(card12);
                                    deck1.addToBottom(card23);
                                    deck1.addToBottom(card24);
                                    deck1.addToBottom(card35);
                                    deck1.addToBottom(card30);
                                    cout << name1 << "'s deck: " << deck1.toDeck() << endl;
                                    cout << name2 << "'s deck: " << deck2.toDeck() << endl;
                                    break;
                                }
                                
                                else {
                                    cout << name2 << " wins the war!" << endl;
                                    deck2.addToBottom(card8);
                                    deck2.addToBottom(card5);
                                    deck2.addToBottom(card2);
                                    deck2.addToBottom(card11);
                                    deck2.addToBottom(card6);
                                    deck2.addToBottom(card17);
                                    deck2.addToBottom(card12);
                                    deck2.addToBottom(card23);
                                    deck2.addToBottom(card18);
                                    deck2.addToBottom(card29);
                                    deck2.addToBottom(card24);
                                    deck2.addToBottom(card35);
                                    deck2.addToBottom(card30);
                                    deck2.addToBottom(card7);
                                    deck2.addToBottom(card3);
                                    deck2.addToBottom(card4);
                                    deck2.addToBottom(card9);
                                    deck2.addToBottom(card4);
                                    deck2.addToBottom(card15);
                                    deck2.addToBottom(card10);
                                    deck2.addToBottom(card27);
                                    deck2.addToBottom(card22);
                                    deck2.addToBottom(card33);
                                    deck2.addToBottom(card28);
                                    cout << name1 << "'s deck: " << deck1.toDeck() << endl;
                                    cout << name2 << "'s deck: " << deck2.toDeck() << endl;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    
        round++;
    }
    string winnerName = getWinnerName();
    cout << endl << winnerName << " wins the game!" << endl << endl;
}
