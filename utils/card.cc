#include "card.h"

Card::Card(int rank, char type) :
    rank{rank},
    type{type}
{}

Card::~Card() {}

int Card::getRank() {
    return rank;
}

bool Card::operator<(shared_ptr<Card> comp) {
  return (rank < comp->rank);
}