#include "subject.h"

Subject::Subject() {}
Subject::~Subject() {}

void Subject::attach( Observer *ob ) {
    observers.emplace_back(ob);
}

void Subject::detach( Observer *ob ) {
    for ( auto it = observers.begin(); it != observers.end(); ++it ) {
        if ( *it == ob ) {
            observers.erase(it);
            break;
        }
    }
}

int Subject::notifyObservers() {  // plays game until reaches a human player or end
    int playerTurn = 0;

    while (!playerTurn) {
        for (auto ob : observers) {
            playerTurn = ob->notify();
        }
    }
    
    return playerTurn - 1;
}

void Subject::modifyObservers(string type, string card) {
    for (auto ob : observers) ob->modify(type, card);
}