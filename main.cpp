
#include <iostream>
#include <string>
#include "Blackjack.h"

using namespace std;

int main() {
    
    Blackjack blackjack;
    blackjack.runBlackjack();
    //Remember to have only one line in main before submission, mentor wants to see a game-controller class that main only runs.
    return 0;
}

//we managed to solve get around the problem of infinite loop when we enter chars for some switch statements. We need to know how to stop
// the same problem when we have to input doubles and the user press on letter button. 