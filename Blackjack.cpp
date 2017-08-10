#include "Blackjack.h"
#include <cstdlib> //for rand()
#include <ctime> //for returning the number of seconds in computer time

Card::Card() {
    face = "0";
    suit = "0";
    value = 0;
    //cout << "default constructor" << endl;
}

Card::Card(string cardFace, string cardSuit) {
    //assigns the paraments above to the two variables face and suit
    face = cardFace;
    suit = cardSuit;

    //Can't use switch statement on strings
    if (face == "Ace") {
        value = 1;
    } else if (face == "2") {
        value = 2;
    } else if (face == "3") {
        value = 3;
    } else if (face == "4") {
        value = 4;
    } else if (face == "5") {
        value = 5;
    } else if (face == "6") {
        value = 6;
    } else if (face == "7") {
        value = 7;
    } else if (face == "8") {
        value = 8;
    } else if (face == "9") {
        value = 9;
    } else if (face == "10" || face == "Jack" || face == "Queen" || face == "King") {
        value = 10;
    };
}

void Card::print() {

    stringstream cardDescription;
    cardDescription << face;
    cardDescription << " of ";
    cardDescription << suit;
    cardDescription << " with a value of ";
    cardDescription << value;

    string cDescription = cardDescription.str();
    cout << cDescription << endl;

}

Session::Session() { //constructor
    cout << "Welcome to Blackjack. A new session is initialized. Please answer the following questions and press enter in order to proceed:" << endl;
    cout << " " << endl;
}

void Session::getName() {
    cout << "What is your name?" << endl;
    cin >> name;
    cout << " " << endl;
}

//We may instead give the player an initial bankroll of 5000 virtual dollars instead! Many blackjack apps adopt this approach.)
//cout << stod("  99.999  ") << endl;

void Session::getInitialBankroll() {
    string dummy;
    while (bankroll < 100) {
        cout << "The minimum amount to pay for chips is $100. How much you would like to pay for chips (no change is allowed in the game)." << endl;
        cin >> bankroll;
        if (!cin.fail()) {
            initialBankroll = bankroll;
            cout << " " << endl;
        } else {
            cin.clear();
            cin >> dummy;
        };

    };
}

void Session::getNumDecks() {
    string dummy;
    cout << "You can play on a table with 1, 2, 4, or 8 decks. Enter 1, 2, 4, or 8 to pick your table!" << endl;
    cin >> numDecks;

    if (cin.fail()) {
        cin.clear();
        cin >> dummy;
    };
    bool validNumDecks;
    while (!validNumDecks) {
        switch (numDecks) {
            case 1:
                validNumDecks = true;
                break;
            case 2:
                validNumDecks = true;
                break;
            case 4:
                validNumDecks = true;
                break;
            case 8:
                validNumDecks = true;
                break;
            default:
                cout << "Unrecognized value. Please enter 1, 2, 4, or 8 to pick your table" << endl;
                validNumDecks = false;
                cin >> numDecks;
                if (cin.fail()) {
                    cin.clear();
                    cin >> dummy;
                };
        };
    };
    numCards = deckSize * numDecks;
    cout << " " << endl;
}

void Session::announce1() {
    stringstream firstMessage;
    firstMessage << "Thank you ";
    firstMessage << name;
    firstMessage << ". You now have chips worth of $";
    firstMessage << bankroll;
    firstMessage << ". You picked a table with ";
    firstMessage << numDecks;
    firstMessage << " decks. Enjoy playing!";

    string fMessage = firstMessage.str();
    cout << fMessage << endl;
}

// we need to return the decks of cards from initializeDecks and input it to shuffleDecks and output the shuffled decks!
// we need to generate a random number between 0 and i
// we need to swap using temp
//replace void with this Card* 

/* pseudocode from https://www.rosettacode.org/wiki/Knuth_shuffle
     for i from last downto 1 do:
        let j = random integer in range 0 ≤ j ≤  i
        swap items[i] with items[j]
 * Generally speaking, there is no need to shuffle the cards before dealing, the program can randomly deal cards from initialized cards
 * You just need to make sure the program remembers which indices were randomly dealt so as to avoid dealing them back again!
 */

Card* Session::initializeDecks() {
    //an array for faces
    string faces[] = {"Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King"};
    //as array for suits
    string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};

    int numCards = deckSize * numDecks;
    // cout << numCards << endl;
    // Initiate an empty array of cards with size equal to numCards
    Card *decks = new Card[numCards];
    //Card decks[numCards]; didn't work
    //Populate the array
    for (int index = 0; index < numCards; index++) {
        decks[index] = Card(faces[index % 13], suits[index % 4]);
    }
    //decks[0].print(); returns initializedCards
    //for (int n = 0; n < 52; n ++) decks[n].print(); //nice to check on decks

    return decks;

}

Card* Session::shuffleDecks(Card *initializedCards) {

    int numCards = deckSize * numDecks;

    srand(time(NULL));
    //srand((unsigned)time(0));
    int j;

    for (int i = numCards - 1; i > 0; i--) {

        j = (rand()) % i;
        Card temp = initializedCards[j];
        initializedCards[j] = initializedCards[i];
        initializedCards[i] = temp;

    };
    return initializedCards;
}

bool Session::reachShuffleLimit(int currentPosition) {
    switch (numDecks) {
        case 1:
            if (currentPosition > numCards - 15) {
                return 1;
            } else {
                return 0;
            }; //4 aces +  4 2's + 4 3's = 12 cards + 2 cards for the dealer + safety card
        case 2:
            if (currentPosition > numCards - 18) {
                return 1;
            } else {
                return 0;
            }; // 8 aces + 7 2's = 15 cards + 2 cards for the dealer + safety card

        case 4:
            if (currentPosition > numCards - 20) {
                return 1;
            } else {
                return 0;
            }; // 12 aces + 5 2's = 17 cards + 2 cards for the dealer + safety card

        case 8:
            if (currentPosition > numCards - 25) {
                return 1;
            } else {
                return 0;
            }; // 22 aces + 2 cards for the dealer + safety card
    }
}

Round::Round(Card *shuffledCards, Session &session) { //constructor
    cout << " " << endl;
    canSplit = ((shuffledCards[session.currentPosition].value == shuffledCards[session.currentPosition + 2].value) ? true : false);
    splitValue = shuffledCards[session.currentPosition].value;
}

/*
void Round::getCurrentBet(Session &session) {
    while (currentBet < 1 || currentBet > 1000) {
        cout << "How much would you like to bet? Minimum bet is $1 and maximum bet is $1000" << endl;
        cin >> currentBet;
        while (currentBet > session.bankroll) {
            cout << "Sorry you don't have enough money in your bankroll to bet that amount." << endl;
            session.getInitialBankroll();
        };
        cout << " " << endl;
    };
}
 */
void Round::getCurrentBet(Session &session) {
    string dummy;
    while (currentBet < 1 || currentBet > 100) {
        cout << "How much would you like to bet? Minimum bet is $1 and maximum bet is $" << flush;
        cout << session.bankroll << endl;
        cin >> currentBet;
        if (cin.fail()) {
            cin.clear();
            cin >> dummy;
        };
        if (currentBet > session.bankroll) {
            cout << "Sorry you don't have enough money in your bankroll to bet that amount." << endl;
            session.getInitialBankroll();
        };
    };
    cout << " " << endl;
}

void Round::printCurrentBet() {
    cout << "Your current bet is $" << flush;
    cout << currentBet << endl;
}

void Round::displayDealerValue() {
    if (dealerAces || (dealerSecondCardFace == "Ace")) {
        dealerAces = 1;
        cout << "Dealer's hand has two possible values: " << flush;
        cout << totalDealer << flush;
        cout << " and " << flush;
        cout << totalDealer + 10 << flush;
        cout << "." << endl;

    } else {
        cout << "Dealer total value is : " << flush;
        cout << totalDealer << endl;

    };
}

void Round::dealPlayer(Card *shuffledCards, Session &session) {
    if (shuffledCards[session.currentPosition].face == "Ace") {
        playerAces = 1;
    };
    totalPlayer += shuffledCards[session.currentPosition].value;

    cout << "You get a " << flush;
    shuffledCards[session.currentPosition].print();


    if (playerAces && (totalPlayer + 10 < 22)) {
        cout << "You have two values: " << flush;
        cout << totalPlayer << flush;
        cout << " and " << flush;
        cout << totalPlayer + 10 << flush;
        cout << "." << endl;
    } else {
        cout << "Your current total value is " << flush;
        cout << totalPlayer << endl;
    };
    session.currentPosition += 1;
    if (playerAces && (((totalPlayer) + 10) < 22)) {
        totalPlayer += 10; // Take the highest for the dealer
    };
}

void Round::dealDealer(Card *shuffledCards, Session & session) {

    if (shuffledCards[session.currentPosition].face == "Ace") {
        dealerAces = 1;
    };
    totalDealer += shuffledCards[session.currentPosition].value;

    cout << "Dealer get a " << flush;
    shuffledCards[session.currentPosition].print();


    if (dealerAces && (totalDealer + 10 < 22)) {
        cout << "Dealer has two values: " << flush;
        cout << totalDealer << flush;
        cout << " and " << flush;
        cout << totalDealer + 10 << flush;
        cout << "." << endl;
    } else {
        cout << "Dealer's current total value is " << flush;
        cout << totalDealer << endl;
    };
    if (dealerAces && (((totalDealer) + 10) < 22)) {
        totalDealer += 10; // Take the highest for the dealer
    };

    session.currentPosition += 1;
}

void Round::outputPlayerInitialHand(Card *shuffledCards, Session & session) {
    cout << "You get " << flush;
    if (shuffledCards[session.currentPosition].face == "Ace") {
        playerAces = 1;
        cout << "Ace of " << flush;
        cout << shuffledCards[session.currentPosition].suit << flush;
        cout << " with a value of 11" << endl;
    } else {
        shuffledCards[session.currentPosition].print();
    };

    cout << "    and " << flush;
    if (shuffledCards[session.currentPosition + 2].face == "Ace") {
        if (playerAces == 0) {
            playerAces = 1;
            cout << "Ace of " << flush;
            cout << shuffledCards[session.currentPosition + 2].suit << flush;
            cout << " with a value of 11" << endl;
        } else {
            shuffledCards[session.currentPosition + 2].print();
        };
    } else {
        shuffledCards[session.currentPosition + 2].print();
    };

    if (hasBlackJackPlayer) {
        cout << "Your total value is " << flush;
        cout << totalPlayer + 10 << endl;
    } else if (!hasBlackJackPlayer && playerAces) {
        cout << "You have two values: " << flush;
        cout << totalPlayer << flush;
        cout << " and " << flush;
        cout << totalPlayer + 10 << flush;
        cout << "." << endl;
    } else if (!hasBlackJackPlayer && !playerAces) {
        cout << "Your total value is " << flush;
        cout << totalPlayer << endl;
    };
}

void Round::outputDealerInitialHand(Card *shuffledCards, Session &session, int card) {
    string dummy;
    if (card == 1) {
        cout << "Dealer's first card is " << flush;
        if (shuffledCards[session.currentPosition + 1].face == "Ace") {
            dealerAces = 1;
            cout << "Ace of " << flush;
            cout << shuffledCards[session.currentPosition + 1].suit << flush;
            cout << " with a value of 1 or 11. Dealer may or may not have a blackjack depending on the hidden card!" << endl;

            if (!hasBlackJackPlayer) {
                cout << " You can insure your current bit in case dealer has a blackjack. The way insurance go is you bet half your current bet, if dealer has a blackjack, you breakeven and lose no money, if dealer doesn't have blackjack, you lose your insurance and continue the game as usual." << endl;
                cout << " Would you like to insure? Please enter 0 for no and 1 for yes." << endl;
                cin >> playerInsure;
                if (cin.fail()) {
                    cin.clear();
                    cin >> dummy;
                };

                switch (playerInsure) {
                    case 1:
                        cout << "You chose to insure your bet using an amount of $" << flush;
                        cout << 0.5 * currentBet << endl;
                    case 0:
                        cout << "You chose not to insure your bet." << endl;
                    default:
                        cout << "Unrecognized value; Please enter 0 for no and 1 for yes." << endl;
                        cin >> playerInsure;
                        if (cin.fail()) {
                            cin.clear();
                            cin >> dummy;
                        };
                };

            };
        } else {
            shuffledCards[session.currentPosition + 1].print();
        };

    } else if (card == 2) {
        cout << "Dealer's second card is " << flush;
        if (shuffledCards[session.currentPosition + 3].face == "Ace") {
            if (dealerAces == 0) {
                dealerAces = 1;
                cout << "Ace of " << flush;
                cout << shuffledCards[session.currentPosition + 3].suit << flush;
                cout << " with a value of 11" << endl;
            } else {
                shuffledCards[session.currentPosition + 3].print();
            };
        } else {
            shuffledCards[session.currentPosition + 3].print();
        };
        // modify based on the dealer
        if (hasBlackJackDealer) {
            cout << "Dealer's total value is " << flush;
            cout << totalDealer + 10 << endl;
        } else if (!hasBlackJackDealer && dealerAces) {
            cout << "Dealer have two values: " << flush;
            cout << totalDealer << flush;
            cout << " and " << flush;
            cout << totalDealer + 10 << flush;
            cout << "." << endl;
        } else if (!hasBlackJackDealer && !dealerAces) {
            cout << "Dealer's total value is " << flush;
            cout << totalDealer << endl;
        };
    };
}

bool Round::intializeRound(Card *shuffledCards, Session & session) {

    if (session.reachShuffleLimit(session.currentPosition)) {
        cout << "=============================================================================" << endl;
        cout << "Oops, the cards need to be shuffled" << endl;
        session.shuffleDecks(shuffledCards);
        session.currentPosition = 0;
        cout << "Cards have been just shuffled, let's continue!" << endl;
        cout << "=============================================================================" << endl;
    };
    // this is to shuffle before the round begins, this is not the best location to do this, take this to game controller when 
    // you implement this new class
    getCurrentBet(session);
    printCurrentBet();

    if (!session.reachShuffleLimit(session.currentPosition)) {
        totalPlayer = shuffledCards[session.currentPosition].value + shuffledCards[session.currentPosition + 2].value;
        totalDealer = shuffledCards[session.currentPosition + 1].value + shuffledCards[session.currentPosition + 3].value;
        hasBlackJackPlayer = (totalPlayer == 11) && ((shuffledCards[session.currentPosition].face == "Ace") || (shuffledCards[session.currentPosition + 2].face == "Ace"));
        hasBlackJackDealer = (totalDealer == 11) && ((shuffledCards[session.currentPosition + 1].face == "Ace") || (shuffledCards[session.currentPosition + 3].face == "Ace"));
        dealerSecondCardFace = shuffledCards[session.currentPosition + 3].face;
        dealerSecondCardSuit = shuffledCards[session.currentPosition + 3].suit;
        dealerSecondCardValue = shuffledCards[session.currentPosition + 3].value;



        outputPlayerInitialHand(shuffledCards, session);


        cout << "=================================================" << endl;
        outputDealerInitialHand(shuffledCards, session, 1);

        if (hasBlackJackPlayer && !hasBlackJackDealer) {
            session.playerWins += 1;
            session.bankroll += 1.5 * currentBet;
            outputDealerInitialHand(shuffledCards, session, 2);

            cout << "You just won a BlackJack, congratulations! " << flush;
            cout << "You won $" << flush;
            cout << 1.5 * currentBet << endl;
            cout << " and your current bankroll goes up to $" << flush;
            cout << session.bankroll << endl;
            cout << "Dealer's second card is " + dealerSecondCardFace + " of " + dealerSecondCardSuit << endl;
            return 0;
        };

        if (!hasBlackJackPlayer && hasBlackJackDealer) {

            outputDealerInitialHand(shuffledCards, session, 2);

            if (playerInsure) {
                cout << "So, dealer has a Blackjack and you don't. But the good news is you insured your bet and you don't lose anything." << endl;
                cout << "Your current bankroll is $" << flush;
                cout << session.bankroll << endl;
            } else {
                session.bankroll -= currentBet;
                cout << "Bummer, dealer has a Blackjack and you don't, You lost your bet, good luck next round." << flush;
                cout << "Your current bankroll goes down to $" << flush;
                cout << session.bankroll << endl;

            };

            return 0;
        };

        if (hasBlackJackPlayer && hasBlackJackDealer) {
            outputDealerInitialHand(shuffledCards, session, 2);
            cout << " What a coincidense, Both you and the dealer got a blackjack, it is a tie, you don't win, you don't lose, your bankroll stay at $" << flush;
            cout << session.bankroll << endl;

            return 0;
        };

        return 1;
    };
}

Dealer::Dealer() {
    cout << " " << endl;
}

void Dealer::dealerTakeOver(Card *shuffledCards, Session &session, Round & round) {
    cout << "Dealer's second card is " + round.dealerSecondCardFace + " of " + round.dealerSecondCardSuit << flush;
    cout << " with a value of " << flush;
    cout << round.dealerSecondCardValue << endl;

    round.displayDealerValue();
    //just added
    if (round.dealerAces && (((round.totalDealer) + 10) < 22)) {
        round.totalDealer += 10; // Take the highest for the dealer
    };
    while ((round.totalDealer <= 17)) { //assuming dealer is risk averse
        if (round.totalDealer == 17 && !round.dealerAces) {
            break;
        } else if (round.totalDealer == 17 && round.dealerAces) {
            cout << "Dealer must hit on soft 17 irrespective to the total value of the player. Dealer hits and gets: " << endl;
            round.dealDealer(shuffledCards, session);
        } else if (round.totalDealer == 16) {
            cout << "Dealer must hit on 16 irrespective to the total value of the player. Dealer hits and gets: " << endl;
            round.dealDealer(shuffledCards, session);
        } else if ((round.totalDealer < 16) && (round.totalDealer <= round.totalPlayer)) {
            cout << "Dealer will hit. Dealer gets: " << endl;
            round.dealDealer(shuffledCards, session);
        } else if ((round.totalDealer < 16) && (round.totalDealer > round.totalPlayer)) {
            cout << "Dealer's total is: " << flush;
            cout << round.totalDealer << endl;
            session.currentPosition += 1;
            break;
        };
    };
    /*
        
     */
    cout << "Your total is: " << flush;
    cout << round.totalPlayer << endl;

    if (round.totalDealer < 22) {

        cout << "Dealer stays on a total value of " << flush;
        cout << round.totalDealer << flush;
        cout << " and " << endl;

        if (round.totalDealer > round.totalPlayer) {
            session.bankroll -= round.currentBet;
            cout << "You lose this round. Your current bankroll is $" << flush;
            cout << session.bankroll << endl;
        } else if (round.totalDealer < round.totalPlayer) {
            session.bankroll += round.currentBet;
            cout << "You win this round. Your current bankroll is $" << flush;
            cout << session.bankroll << endl;
            session.playerWins += 1;
        } else {
            cout << "The result of this round is a push. Your current bankroll is $" << flush;
            cout << session.bankroll << endl;
        };
    } else if (round.totalDealer > 21) {
        session.bankroll += round.currentBet;
        cout << "Dealer busts and you win this round. Your current bankroll is $" << flush;
        cout << session.bankroll << endl;
        session.playerWins += 1;
    };
}

void Dealer::dealerTakeOverSplitMode(Card *shuffledCards, Session &session, Round & round) {
    cout << "Dealer's second card is " + round.dealerSecondCardFace + " of " + round.dealerSecondCardSuit << flush;
    cout << " with a value of " << flush;
    cout << round.dealerSecondCardValue << endl;
    round.displayDealerValue();

    while ((round.totalDealer <= 17)) { //assuming dealer is risk averse
        if (round.totalDealer == 17 && !round.dealerAces) {
            break;
        } else if (round.totalDealer == 17 && round.dealerAces) {
            cout << "Dealer must hit on soft 17 irrespective to the total value of the player." << endl;
            round.dealDealer(shuffledCards, session);
        } else if (round.totalDealer == 16) {
            cout << "Dealer must hit on 16 irrespective to the total value of the player." << endl;
            round.dealDealer(shuffledCards, session);
        } else if ((round.totalDealer < 16) && ((round.totalDealer <= round.totalPlayersplitHand1) || (round.totalDealer <= round.totalPlayersplitHand2))) {
            cout << "Dealer will hit." << endl;
            round.dealDealer(shuffledCards, session);
        } else if ((round.totalDealer < 16) && ((round.totalDealer > round.totalPlayersplitHand1) && (round.totalDealer > round.totalPlayersplitHand2))) {
            cout << "Dealer total is: " << flush;
            cout << round.totalDealer << endl;
            session.currentPosition += 1;
        };
    };

    if (round.totalDealer < 22) {
        round.totalPlayersplitHand1 = ((round.totalPlayersplitHand1 > 21) ? 0 : round.totalPlayersplitHand1);
        round.totalPlayersplitHand2 = ((round.totalPlayersplitHand2 > 21) ? 0 : round.totalPlayersplitHand2);

        bool playerWinTwoHands = ((round.totalDealer < round.totalPlayersplitHand1) && (round.totalDealer < round.totalPlayersplitHand2));
        bool playerWinOneHand = (((round.totalDealer < round.totalPlayersplitHand1) && (round.totalDealer == round.totalPlayersplitHand2)) ||
                ((round.totalDealer == round.totalPlayersplitHand1) && (round.totalDealer < round.totalPlayersplitHand2)));
        bool playerLoseTwoHands = ((round.totalDealer > round.totalPlayersplitHand1) && (round.totalDealer > round.totalPlayersplitHand2));
        bool playerLoseOneHand = (((round.totalDealer > round.totalPlayersplitHand1) && (round.totalDealer == round.totalPlayersplitHand2)) ||
                ((round.totalDealer == round.totalPlayersplitHand1) && (round.totalDealer > round.totalPlayersplitHand2)));

        cout << "Dealer stays  on a total value of " << flush;
        cout << round.totalDealer << flush;
        cout << " and " << endl;


        if (playerWinTwoHands) {
            session.bankroll += 2 * round.currentBet;
            cout << "You win the two hands. Your current bankroll is $" << flush;
            cout << session.bankroll << endl;
            session.playerWins += 1;

        } else if (playerWinOneHand) {
            session.bankroll += round.currentBet;
            cout << "You win one hand and push the other. Your current bankroll is $" << flush;
            cout << session.bankroll << endl;
            session.playerWins += 1;

        } else if (playerLoseTwoHands) {
            session.bankroll -= 2 * round.currentBet;
            cout << "You lose the two hands. Your current bankroll is $" << flush;
            cout << session.bankroll << endl;

        } else if (playerLoseOneHand) {
            session.bankroll -= round.currentBet;
            cout << "You lose one hand and push the other. Your current bankroll is $" << flush;
            cout << session.bankroll << endl;
        } else {
            cout << "You win one hand and lose the other. Your current bankroll is $" << flush;
            cout << session.bankroll << endl;
        };

    } else if (round.totalDealer > 21) {
        bool playerWinTwoHands = ((round.totalPlayersplitHand1 < 22) && (round.totalPlayersplitHand2 < 22));
        bool playerWinOneHand = ((round.totalPlayersplitHand1 < 22) ^ (round.totalPlayersplitHand2 < 22));

        cout << "Dealer busts and " << flush;

        if (playerWinTwoHands) {
            session.bankroll += 2 * round.currentBet;
            cout << "you win the two hands. Your current bankroll is $" << flush;
            cout << session.bankroll << endl;
            session.playerWins += 1;

        } else if (playerWinOneHand) {
            session.bankroll += round.currentBet;
            cout << "you win one hand and push the other. Your current bankroll is $" << flush;
            cout << session.bankroll << endl;
            session.playerWins += 1;

        };
    };

}

Player::Player() {
    cout << " " << endl;
}

void Player::getPlayerDecision2(Card *shuffledCards, Session &session, Round &round, Dealer & dealer) {
    string dummy;

    round.dealPlayer(shuffledCards, session);

    if (round.totalPlayer > 21) {
        session.bankroll -= round.currentBet;
        cout << "You busted and lost this round. Your current bankroll is $" << flush;
        cout << session.bankroll << endl;
        cout << "Dealer's second card is " + round.dealerSecondCardFace + " of " + round.dealerSecondCardSuit << endl;
    } else {
        cout << "You can enter: 1 for hit, 2 for stay." << endl;
        cin >> playerDecision2;
        if (cin.fail()) {
            cin.clear();
            cin >> dummy;
        };
        cout << " " << endl;
        bool validDecision2 = 0;
        while (!validDecision2) {

            switch (playerDecision2) {
                case 1:
                    validDecision2 = true;
                    cout << "You decided to hit." << endl;
                    this -> getPlayerDecision2(shuffledCards, session, round, dealer); //recursive call. As long as the parameters are the same, iterative solution is better than recursive solution. Recursive is better when the input parameters are different
                    break;
                case 2:
                    validDecision2 = true;
                    if (round.playerAces && (((round.totalPlayer) + 10) < 22)) {
                        round.totalPlayer += 10; // Take the highest for the player
                        cout << "You decided to stay on a total value of " << flush;
                        cout << round.totalPlayer << endl;
                    } else {
                        cout << "You decided to stay on a total value of " << flush;
                        cout << round.totalPlayer << endl;
                    };
                    dealer.dealerTakeOver(shuffledCards, session, round);
                    break;
                default:
                    cout << "Unrecognized value. Please enter 1 for hit, 2 for stay." << endl;
                    cin >> playerDecision2;
                    if (cin.fail()) {
                        cin.clear();
                        cin >> dummy;
                    };
                    cout << " " << endl;
                    bool validDecision2 = 0;
            };
        };
    };
};

int Player::getPlayerDecisionsSplitMode(Card *shuffledCards, Session &session, Round & round) {
    string dummy;

    if (round.totalPlayer > 21) {
        cout << "you busted this hand." << endl;
    } else {
        cout << "You can enter: 1 for hit, 2 for stay." << endl;
        cin >> playerDecision2;
        if (cin.fail()) {
            cin.clear();
            cin >> dummy;
        };
        cout << " " << endl;
        bool validDecision2 = 0;
        while (!validDecision2) {
            switch (playerDecision2) {
                case 1:
                    validDecision2 = true;
                    cout << "You decided to hit." << endl;
                    round.dealPlayer(shuffledCards, session);
                    this -> getPlayerDecisionsSplitMode(shuffledCards, session, round); //recursive call. As long as the parameters are the same, iterative solution is better than recursive solution. Recursive is better when the input parameters are different
                    break;
                case 2:
                    validDecision2 = true;
                    if (round.playerAces && (((round.totalPlayer) + 10) < 22)) {
                        round.totalPlayer += 10; // Take the highest for the player
                        cout << "You decided to stay on a total value of " << flush;
                        cout << round.totalPlayer << endl;
                    } else {
                        cout << "You decided to stay on a total value of " << flush;
                        cout << round.totalPlayer << endl;
                    };
                    break;
                default:
                    cout << "Unrecognized value. Please enter 1 for hit, 2 for stay." << endl;
                    cin >> playerDecision2;
                    if (cin.fail()) {
                        cin.clear();
                        cin >> dummy;
                    };
                    cout << " " << endl;
                    bool validDecision2 = 0;
            };
        };

    };
    return round.totalPlayer;
}

void Player::getPlayerDecision1(Card *shuffledCards, Session &session, Round &round, Dealer & dealer) {
    string dummy;

    if (!round.hasBlackJackPlayer && !round.hasBlackJackDealer) {
        if (round.playerInsure) {
            cout << "You lose your insurance cause dealer doesn't have a blackjack!" << endl;
            session.bankroll -= 0.5 * round.currentBet;
            cout << "Your current bankroll is $" << flush;
            cout << session.bankroll << endl;
        };
        if (shuffledCards[session.currentPosition].value == shuffledCards[session.currentPosition + 2].value) {
            round.canSplit = 1;
        };

        session.currentPosition += 4;
        cout << "Player will make some decisions before the dealer takes over." << endl;
        switch (round.canSplit) {
            case 1:
                if (session.bankroll >= 2 * round.currentBet) {
                    cout << "You can enter: 1 for hit, 2 for stay, 3 for surrender, 4 for double, or 5 for split!" << endl;
                    cin >> playerDecision1;
                    if (cin.fail()) {
                        cin.clear();
                        cin >> dummy;
                    };
                    cout << " " << endl;
                } else {
                    while (!((playerDecision1 == 1) || (playerDecision1 == 2) ||  (playerDecision1 == 3))) {
                        cout << "You can enter: 1 for hit, 2 for stay, 3 for surrender. You can't double or split because you don't have enough cash in your bankroll to double your current bet." << endl;
                        cin >> playerDecision1;
                        if (cin.fail()) {
                            cin.clear();
                            cin >> dummy;
                        };
                        cout << " " << endl;
                    };
                };
                break;
            case 0:
                if (session.bankroll >= 2 * round.currentBet) {
                    cout << "You can enter: 1 for hit, 2 for stay, 3 for surrender, 4 for double." << endl;
                    cin >> playerDecision1;
                    if (cin.fail()) {
                        cin.clear();
                        cin >> dummy;
                    };
                    cout << " " << endl;
                } else {
                    while (!((playerDecision1 == 1) || (playerDecision1 == 2) ||  (playerDecision1 == 3))) {
                        cout << "You can enter: 1 for hit, 2 for stay, 3 for surrender. You can't double or split because you don't have enough cash in your bankroll to double your current bet." << endl;
                        cin >> playerDecision1;
                        if (cin.fail()) {
                            cin.clear();
                            cin >> dummy;
                        };
                        cout << " " << endl;
                    };
                };
                break;
        };

        bool validDecision1 = 0;
        while (!validDecision1) {
            switch (playerDecision1) {
                case 1:
                    validDecision1 = true;
                    cout << "You decided to hit." << endl;
                    getPlayerDecision2(shuffledCards, session, round, dealer);
                    break;
                case 2:
                    validDecision1 = true;
                    if (round.playerAces && (((round.totalPlayer) + 10) < 22)) {
                        round.totalPlayer += 10; // Take the highest for the player
                        cout << "you decided to stay on a total value of " << flush;
                        cout << round.totalPlayer << endl;
                    } else {
                        cout << "you decided to stay on a total value of " << flush;
                        cout << round.totalPlayer << endl;
                    };
                    dealer.dealerTakeOver(shuffledCards, session, round);
                    break;
                case 3:
                    validDecision1 = true;      
                    session.bankroll -= 0.5 * round.currentBet;
                    cout << "You decided to surrender. Your current bankroll is $" << flush;
                    cout << session.bankroll << endl;
                    cout << "Dealer's second card is " + round.dealerSecondCardFace + " of " + round.dealerSecondCardSuit << endl;
                    break;

                case 4:
                    validDecision1 = true;
                    round.currentBet *= 2;
                    cout << "You decided to double. You will only be dealt one more card. Your current bet is doubled to $" << flush;
                    cout << round.currentBet << endl;
                    //logic
                    if (shuffledCards[session.currentPosition].face == "Ace") {
                        round.playerAces = 1;
                    };
                    round.totalPlayer += shuffledCards[session.currentPosition].value;
                    if (round.playerAces && (((round.totalPlayer) + 10) < 22)) {
                        round.totalPlayer += 10; // Take the highest for the dealer
                    };
                    cout << "You get a " << flush;
                    shuffledCards[session.currentPosition].print();
                    cout << "Your current total value is " << flush;
                    cout << round.totalPlayer << endl;
                    session.currentPosition += 1;
                    if (round.totalPlayer <= 21) {
                        cout << "Dealer takes over at this point." << endl;
                        dealer.dealerTakeOver(shuffledCards, session, round);
                        break;
                    };
                    if (round.totalPlayer > 21) {
                        session.bankroll -= round.currentBet;
                        cout << "you busted and lost this round. Your current bankroll is" << flush;
                        cout << session.bankroll << endl;
                        break;
                    };

                case 5:
                    switch (round.canSplit) {
                        case 1:
                            // we need to integrate two hands with the dealer!
                            validDecision1 = true;
                            round.choseSplit = 1;

                            cout << "You decided to split. " << endl;

                            round.totalPlayer = round.splitValue;
                            cout << "First hand situation:" << endl;
                            round.dealPlayer(shuffledCards, session);
                            round.splitValueFirst = round.totalPlayer;

                            round.totalPlayer = round.splitValue;
                            cout << "Second hand situation:" << endl;
                            round.dealPlayer(shuffledCards, session);
                            round.splitValueSecond = round.totalPlayer;


                            if (round.splitValue != 1) { // player is only dealt a card per each hand and that's it, similar to doubling

                                cout << "Let's play your first hand with a current total of " << flush;
                                cout << round.splitValueFirst << endl;
                                round.totalPlayer = round.splitValueFirst;
                                round.totalPlayersplitHand1 = getPlayerDecisionsSplitMode(shuffledCards, session, round);

                                cout << "Total first hand is " << flush;
                                cout << round.totalPlayersplitHand1 << endl;

                                cout << "Let's play your second hand with a current total of " << flush;
                                cout << round.splitValueSecond << endl;
                                round.totalPlayer = round.splitValueSecond;
                                round.totalPlayersplitHand2 = getPlayerDecisionsSplitMode(shuffledCards, session, round);

                                cout << "Total second hand is " << flush;
                                cout << round.totalPlayersplitHand2 << endl;

                            };

                            if (round.totalPlayersplitHand1 > 21 && round.totalPlayersplitHand2 > 21) {
                                session.bankroll -= 2 * round.currentBet;
                                cout << "you busted the two hands and your current bankroll is $" << flush;
                                cout << session.bankroll << endl;
                            } else {
                                dealer.dealerTakeOverSplitMode(shuffledCards, session, round);
                            };
                            continue; //added instead of continue, caused an error 

                        case 0:
                            cout << "You can't split because the two cards have different values. Please enter 1 for hit, 2 for stay, 3 for surrender, or 4 for double!" << endl;
                            validDecision1 = false;
                            cin >> playerDecision1;
                    };
                default:
                    /*
                     * It's because the input is trying to get an integer. When the input is not an integer, the input is left in the 
                     * buffer, so next time around in the loop the same input is still there.
                     * Also, you are not calling the flushCin function in the default case, you are declaring it. You might want to 
                     * remove the void keyword. I guess it does the correct thing? (I.e. calling std::cin.ignore() and std::cin::clear().)
                     */
                    cout << "Unrecognized value. Please enter 1 for hit, 2 for stay, 3 for surrender, 4 for double, or 5 if you can and want to split!" << endl;
                    validDecision1 = false;
                    cin >> playerDecision1;
                    if (cin.fail()) {
                        cin.clear();
                        cin >> dummy;
                    };
                    break;
            };
        };
    };
}

Blackjack::Blackjack() {
    cout << " " << endl;
}

void Blackjack::runBlackjack() {

    Session session;
    session.getName();
    session.getInitialBankroll();
    session.getNumDecks();
    session.announce1();

    Card *unshuffledDecks = session.initializeDecks();
    Card *shuffledDecks = session.shuffleDecks(unshuffledDecks);

    Dealer dealer;
    Player player;

    while (session.bankroll > 0) {

        Round round(shuffledDecks, session);

        continueRound = round.intializeRound(shuffledDecks, session);
        if (continueRound) {
            player.getPlayerDecision1(shuffledDecks, session, round, dealer);
        } else {
            session.currentPosition += 4;
        };
        cout << "Current position in the deck is " << flush;
        cout << session.currentPosition << endl;

        session.roundNumber += 1;

        cout << "=============================================================================" << endl;
        cout << "Player won " << flush;
        cout << session.playerWins << flush;
        cout << " out of a total " << flush;
        cout << session.roundNumber << flush;
        cout << " rounds. The player winning percentage is " << flush;
        cout << (session.playerWins / session.roundNumber)*100 << flush;
        cout << "%." << endl;
        cout << "player achieved an ROI of " << flush;
        cout << ((session.bankroll - session.initialBankroll) / session.initialBankroll)*100 << flush;
        cout << "%." << endl;
        cout << "=============================================================================" << endl;

    };

    delete [] shuffledDecks;
}

