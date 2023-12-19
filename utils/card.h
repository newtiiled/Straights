#ifndef CARD_H
#define CARD_H

#include <vector>
#include <memory>
using namespace std;

struct Card {
    int rank;
    char type;

  public:
    explicit Card(int rank, char type);
    ~Card();

    int getRank();
    bool operator<(shared_ptr<Card> comp);
};

#endif