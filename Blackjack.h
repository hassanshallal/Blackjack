#include <iostream>
#include <sstream>
using namespace std;

#ifndef BLACKJACK_H
#define BLACKJACK_H



//Card class

class Card {
public:
    //create two card variables face and suit
    string face;
    string suit;
    int value;
    //default constructor
    Card();
    //constructor with two parameters: face and suite;
    Card(string cardFace, string cardSuit);
    //print function
    void print();
};

class Session {
private:

public:

    //instance variables
    string name;
    int initialBankroll;
    double bankroll; // If you initialize bankroll to an int and the user enters 100.5 or any fraction, a bug obtains. Use double to avoid this bug.
    int numDecks;
    const int deckSize = 52; //hint by Mentor
    int numCards;
    int currentPosition = 0;
    double roundNumber = 0;
    double playerWins = 0;

    //methods
    Session(); //constructor
    void getName();
    void getInitialBankroll();
    void getNumDecks();
    void announce1();
    Card* initializeDecks();
    Card* shuffleDecks(Card *initializedCards); // replace void with this Card*
    bool reachShuffleLimit(int currentPosition);
};

class Round {// we are repeating the announcement of Session constructor
public:
    //Instance variables
    double currentBet = 0;
    bool canSplit = 0;
    bool choseSplit = 0;
    int splitValue;
    int splitValueFirst;
    int splitValueSecond;
    
    int totalPlayer;
    int totalPlayersplitHand1;
    int totalPlayersplitHand2;
    
    int totalDealer;
    
    bool hasBlackJackPlayer = 0;
    bool hasBlackJackDealer = 0;
    bool playerAces = 0; // true in case th player has at least one ace
    bool dealerAces = 0; // true in case the dealer has at least one ace
    
    int playerInsure = 0;
    string dealerSecondCardFace;
    string dealerSecondCardSuit;
    int dealerSecondCardValue;
    

    
    
    //Methods
    Round(Card *shuffledCards, Session &session);

    void getCurrentBet(Session &session);
    void printCurrentBet();

    void displayDealerValue();
   
    
    void dealPlayer(Card *shuffledCards, Session &session);
    void dealDealer(Card *shuffledCards, Session &session);
    void outputPlayerInitialHand(Card *shuffledCards, Session &session);
    void outputDealerInitialHand(Card *shuffledCards, Session &session, int hand);
    bool intializeRound(Card *shuffledCards, Session &session); //return 0 to start a new round or 1 to continue the round



};

class Dealer {
    
   
public:
    Dealer();
    void dealerTakeOver(Card *shuffledCards, Session &session, Round &round);
    void dealerTakeOverSplitMode(Card *shuffledCards, Session &session, Round &round);

};

class Player {
public:
    //instance variables
    int playerDecision1;
    int playerDecision2;

    //methods
    Player();
    void getPlayerDecision1(Card *shuffledCards, Session &session, Round &round, Dealer &dealer);
    void getPlayerDecision2(Card *shuffledCards, Session &session, Round &round, Dealer &dealer);
    int getPlayerDecisionsSplitMode(Card *shuffledCards, Session &session, Round &round);
    
};

class Blackjack {
public:
    bool continueRound;
    
    Blackjack();
    void runBlackjack(); 

};

#endif /* BLACKJACK_H */

// try thinking about a class responsible for collecting input from user
// class deck with iterator design pattern, 
// add more encapsulation
// add a new class for player and keep dealer and client
// Use state design pattern to represent your scenarios