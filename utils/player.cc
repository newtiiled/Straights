#include "player.h"
#include "card.h"

Player::Player(char type, vector<shared_ptr<Card>> cards) :
    type{type},
    hand{cards}
{}

Player::~Player() {}