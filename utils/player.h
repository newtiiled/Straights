#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <memory>
using namespace std;

class Card;

struct Player {
    char type;
    vector<shared_ptr<Card>> hand;
    vector<shared_ptr<Card>> discard;

  public:
    explicit Player(char type, vector<shared_ptr<Card>> cards);
    ~Player();
};

#endif