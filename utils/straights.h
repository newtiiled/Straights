#ifndef STRAIGHTS_H
#define STRAIGHTS_H

#include "subject.h"
#include <vector>
#include <memory>
#include <string>
using namespace std;

class GameSession;
class Card;

struct Straights : public Subject {
    unique_ptr<GameSession> game;

  public:
    Straights(unsigned seed, char one, char two, char three, char four);
    ~Straights();
    
    // make len+1 if computer
    // getState also updates to next turn if it is computer player
    // if end of round and no winners, getState also reshuffles and starts new round
    vector<vector<shared_ptr<Card>>> getState(); // deck, table, hand, legal vectors
    // getState returns data len:
    //   (7) if player turn && not new round
    //   (8) if player turn && new round
    //   (9) if comp turn && not new round
    //   (10) if comp turn && new round
    //   (1) if game end
    void setState(string type, string card = ""); // call method directly
    // setState also calls modifyObservers to display the card played
};

// if player but end game, getState must call findWinner

#endif