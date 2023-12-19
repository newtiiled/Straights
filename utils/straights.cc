#include "straights.h"
#include "gameSession.h"
#include "player.h"
#include "card.h"

Straights::Straights(unsigned seed, char one, char two, char three, char four) :
    game{make_unique<GameSession>(seed, one, two, three, four)}
{}

Straights::~Straights() {}

vector<vector<shared_ptr<Card>>> Straights::getState() {
    vector<vector<shared_ptr<Card>>> data;
    bool newRou = false;
    vector<shared_ptr<Card>> newRouInd;  // if newRound has been called

    if (game->types[game->curPlayer - 1] == 'h'
        && game->players[game->curPlayer - 1]->hand.empty()) {  // if player turn but end round (empty hand)
        game->findWinner();

        if (!game->winner.empty()) {  // if winner, data len (1)
            vector<shared_ptr<Card>> temp;
            for (auto n : game->winner) {
                temp.push_back(make_shared<Card>(n, 'W'));
            }
            data.push_back(temp);
            return data;
        }

        game->newRound();  // if no winner at end round
        newRou = true;
    }

    data.push_back(game->deck);  // get deck (1)
    for (int i = 0; i < 4; i++) {  // get table (5)
        data.push_back(game->table[i]);
    }
    data.push_back(game->players[game->curPlayer - 1]->hand); // get hand (6)
    data.push_back(game->legalPlays());  // legal plays of player (7)

    if (game->types[game->curPlayer - 1] == 'h') {  // if player turn
        if (newRou) {
            data.push_back(newRouInd);
        }
        return data;  // data len (7 or 8)
    }

    game->nextPlay(); // if comp turn

    if (!game->winner.empty()) {  // if winner, data len (1)
        data.clear();
        vector<shared_ptr<Card>> temp;
        for (auto n : game->winner) {
            temp.push_back(make_shared<Card>(n, 'W'));
        }
        data.push_back(temp);
        return data;
    }

    if (data[5].empty()) {  // if no winner but end round
        data.clear();
        game->newRound();
        data = getState();  // will always be either (7) or (9)
        data.push_back(newRouInd);
        return data;
    }

    // if normal comp turn
    vector<shared_ptr<Card>> curPlay;  // info for current play by comp
    curPlay.push_back(game->curCard);
    curPlay.push_back(make_shared<Card>(game->playType, 'T'));
    curPlay.push_back(make_shared<Card>(game->curPlayer, 'P'));
    data.push_back(curPlay);  // a comp play (8)
    data.push_back(newRouInd);  // to differentiate from player turn (9)
    game->nextPlayer();

    return data;  // data len (9 or 10)
}

void Straights::setState(string type, string card) {
    shared_ptr<Card> temp;
    if (type == "play" || type == "discard") {
        int n = card[0] - '0';
        if (card[0] == 'A') {
            n = 1;
        } else if (card[0] == 'T') {
            n = 10;
        } else if (card[0] == 'J') {
            n = 11;
        } else if (card[0] == 'Q') {
            n = 12;
        } else if (card[0] == 'K') {
            n = 13;
        }
        temp = game->findCard(n, card[1]);
    }

    if (type == "play") {
        game->playCard(temp);
        game->nextPlayer();
    } else if (type == "discard") {
        game->discardCard(temp);
        game->nextPlayer();
    }
    
    modifyObservers(type, card);
    if (type == "ragequit") {
        game->ragequit();
    }
}