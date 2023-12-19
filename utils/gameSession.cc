#include "gameSession.h"
#include "card.h"
#include "player.h"
#include <algorithm>
#include <random>
#include <chrono>
#include <stdexcept>

class InvalidPlay{};
class InvalidDiscard{};

GameSession::GameSession(unsigned seed, char one, char two, char three, char four) :
    seed{seed},
    types{one, two, three, four}
{
    for (int i = 0; i < 4; i++) {  // create standard unshuffled deck
        for (int j = 1; j < 14; j++) {
            if (i == 3) {
                deck.push_back(make_shared<Card>(j, 'S'));
                if (j == 7) {
                    allLegal.push_back(deck.back());
                }
            } else if (i == 0) {
                deck.push_back(make_shared<Card>(j, 'C'));
            } else if (i == 2) {
                deck.push_back(make_shared<Card>(j, 'H'));
            } else {
                deck.push_back(make_shared<Card>(j, 'D'));
            }
        }
    }

    newRound();
}

GameSession::~GameSession() {}

void GameSession::newRound() {
    shuffleDeck();
    table.clear();
    vector<shared_ptr<Card>> empty;
    for (int i = 0; i < 4; i++) {
        table.push_back(empty);
    }

    vector<shared_ptr<Card>> first(deck.begin(), deck.begin() + 13);
    vector<shared_ptr<Card>> second(deck.begin() + 13, deck.begin() + 26);
    vector<shared_ptr<Card>> third(deck.begin() + 26, deck.begin() + 39);
    vector<shared_ptr<Card>> fourth(deck.begin() + 39, deck.begin() + 52);

    players.clear();
    players.push_back(make_unique<Player>(types[0], first));
    players.push_back(make_unique<Player>(types[1], second));
    players.push_back(make_unique<Player>(types[2], third));
    players.push_back(make_unique<Player>(types[3], fourth));

    shared_ptr<Card> startCard = findCard(7, 'S');

    if (find(first.begin(), first.end(), startCard) != first.end()) {
        curPlayer = 1;
    } else if (find(second.begin(), second.end(), startCard) != second.end()) {
        curPlayer = 2;
    } else if (find(third.begin(), third.end(), startCard) != third.end()) {
        curPlayer = 3;
    } else {
        curPlayer = 4;
    }
}

shared_ptr<Card> GameSession::findCard(int rank, char type) {
    for (int i = 0; i < 52; i++) {
        if (deck[i]->rank == rank && deck[i]->type == type) {
            return deck[i];
        }
    }
    return nullptr;
}

vector<shared_ptr<Card>> GameSession::legalPlays() {
    vector<shared_ptr<Card>> legal;

    for (auto pl : players[curPlayer - 1]->hand) {
        for (auto ca : allLegal) {
            if (ca == pl) {
                legal.push_back(pl);
            }
        }
    }

    return legal;
}

void GameSession::play(shared_ptr<Card> card) {
    players[curPlayer - 1]->hand.erase(
        remove(players[curPlayer - 1]->hand.begin(),
                players[curPlayer - 1]->hand.end(),
                card),
        players[curPlayer - 1]->hand.end());  // erase card from player hand
    
    int n = 0;  // find correct type of card
    if (card->type == 'C') {
        n = 1;
    } else if (card->type == 'H') {
        n = 2;
    } else if (card->type == 'D') {
        n = 3;
    }
    table[n].push_back(card);  // put card on table
    sort(table[n].begin(), table[n].end());
    curCard = card;
    playType = 1;

    allLegal.erase(remove(allLegal.begin(), allLegal.end(), card),
                    allLegal.end()); // remove cur card from legal cards
    
    if (card->rank == 7 && card->type == 'S') {  // if card was 7S
        allLegal.push_back(findCard(7, 'C'));
        allLegal.push_back(findCard(7, 'H'));
        allLegal.push_back(findCard(7, 'D'));
    }
    if (card->rank >= 7 && card->rank != 13) {  // add legal card
        if (card->type == 'S') {
            allLegal.push_back(findCard(card->rank + 1, 'S'));
        } else if (card->type == 'C') {
            allLegal.push_back(findCard(card->rank + 1, 'C'));
        } else if (card->type == 'H') {
            allLegal.push_back(findCard(card->rank + 1, 'H'));
        } else {
            allLegal.push_back(findCard(card->rank + 1, 'D'));
        }
    }
    if (card->rank <= 7 && card->rank != 1) {  // add legal card
        if (card->type == 'S') {
            allLegal.push_back(findCard(card->rank - 1, 'S'));
        } else if (card->type == 'C') {
            allLegal.push_back(findCard(card->rank - 1, 'C'));
        } else if (card->type == 'H') {
            allLegal.push_back(findCard(card->rank - 1, 'H'));
        } else {
            allLegal.push_back(findCard(card->rank - 1, 'D'));
        }
    }
}

void GameSession::discard(shared_ptr<Card> card) {
    players[curPlayer - 1]->hand.erase(
        remove(players[curPlayer - 1]->hand.begin(),
                players[curPlayer - 1]->hand.end(),
                card),
        players[curPlayer - 1]->hand.end());  // erase card from player hand
    players[curPlayer - 1]->discard.push_back(card);  // add to discard pile
    curCard = card;
    playType = 0;
}

void GameSession::nextPlay() {
    vector<shared_ptr<Card>> legal = legalPlays();

    if (!legal.empty()) {  // if play
        shared_ptr<Card> card = legal.front();
        play(card);

    } else if (!players[curPlayer - 1]->hand.empty()) {  // if discard
        shared_ptr<Card> card = players[curPlayer - 1]->hand.front();
        discard(card);

    } else {  // if round done
        findWinner();
    }
}

void GameSession::findWinner() {
    int w = 0;
    int x = 0;
    int y = 0;
    int z = 0;
    if (!oldScores.empty()) {  // if there were prev rounds, add oldScores to scores
        w += oldScores[0];
        x += oldScores[1];
        y += oldScores[2];
        z += oldScores[3];
    }

    for (auto ca : players[0]->discard) {
        w += ca->rank;
    }
    for (auto ca : players[1]->discard) {
        x += ca->rank;
    }
    for (auto ca : players[2]->discard) {
        y += ca->rank;
    }
    for (auto ca : players[3]->discard) {
        z += ca->rank;
    }

    if (w >= 80 || x >= 80 || y >= 80 || z >= 80) {
        if (w <= x && w <= y && w <= z) {
            winner.push_back(1);
        }
        if (x <= w && x <= y && x <= z) {
            winner.push_back(2);
        }
        if (y <= w && y <= x && y <= z) {
            winner.push_back(3);
        }
        if (z <= w && z <= x && z <= y) {
            winner.push_back(4);
        }
        return;
    }

    oldScores.clear();  // if winner not found yet
    oldScores.push_back(w);
    oldScores.push_back(x);
    oldScores.push_back(y);
    oldScores.push_back(z);
}

void GameSession::nextPlayer() {
    curPlayer++;
    if (curPlayer == 5) {
        curPlayer = 1;
    }
}

void GameSession::playCard(shared_ptr<Card> card) {
    vector<shared_ptr<Card>> legal = legalPlays();

    if (find(legal.begin(), legal.end(), card) != legal.end()) {
        play(card);
    } else {
        throw InvalidPlay{};
    }
}

void GameSession::discardCard(shared_ptr<Card> card) {
    vector<shared_ptr<Card>> legal = legalPlays();

    if (legal.empty()) {
        discard(card);
    } else {
        throw InvalidDiscard{};
    }
}

void GameSession::ragequit() {
    players[curPlayer - 1]->type = 'c';
    types[curPlayer - 1] = 'c';
}

void GameSession::shuffleDeck() {
    default_random_engine rng{seed};
    shuffle( deck.begin(), deck.end(), rng );
}