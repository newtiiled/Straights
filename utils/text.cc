#include "text.h"
#include "gameSession.h"
#include "player.h"
#include "card.h"

#include <iostream>

Text::Text(shared_ptr<Straights> game) :
    subject{game},
    start{true}
{
    subject->attach( this );
}

Text::~Text() {
    subject->detach( this );
}

char Text::rankChar(int n) {
    if (n == 1) {
        return 'A';
    } else if (n == 10) {
        return 'T';
    } else if (n == 11) {
        return 'J';
    } else if (n == 12) {
        return 'Q';
    } else if (n == 13) {
        return 'K';
    }
    return '0' + n;
}

int Text::notify() {
    vector<vector<shared_ptr<Card>>> data = subject->getState();
    size_t dataSize = data.size();

    if (dataSize == 1) {  // if game end
        size_t sizeDiscard;
        int oldScore;
        int sumDiscard;
        char b;

        for (int i = 0; i < 4; i++) {
            cout << "Player" << i + 1 << "'s discards:";
            sizeDiscard = subject->game->players[i]->discard.size();
            for (size_t j = 0; j < sizeDiscard; j++) {
                b = rankChar(subject->game->players[i]->discard[j]->rank);
                cout << " " 
                        << b
                        << subject->game->players[i]->discard[j]->type;
            }

            oldScore = 0;
            cout << "\nPlayer" << i + 1 << "'s score: ";
            if (!subject->game->oldScores.empty()) {
                oldScore = subject->game->oldScores[i];
            }
            cout << oldScore << " + ";
            sumDiscard = 0;
            for (size_t j = 0; j < sizeDiscard; j++) {
                sumDiscard += subject->game->players[i]->discard[j]->getRank();
            }
            cout << sumDiscard << " = " << oldScore + sumDiscard << endl;
        }

        for (auto n : data[0]) {
            cout << "Player" << n->rank << " wins!" << endl;
        }

        return 2;
    }

    if (start || dataSize == 8 || dataSize == 10) {  // if new round happen
        cout << "A new round begins. It's Player";
        if (dataSize == 10 || (start && dataSize == 9)) {
            cout << data[7][2]->rank;
        } else {
            cout << subject->game->curPlayer;
        }
        cout << "'s turn to play." << endl;
        start = false;
    }
    
    char c;
    cout << "Cards on the table:\n" << "Clubs:";
    for (auto n : data[2]) {
        c = rankChar(n->rank);
        cout << " " << c;
    }
    cout << "\nDiamonds:";
    for (auto n : data[4]) {
        c = rankChar(n->rank);
        cout << " " << c;
    }
    cout << "\nHearts:";
    for (auto n : data[3]) {
        c = rankChar(n->rank);
        cout << " " << c;
    }
    cout << "\nSpades:";
    for (auto n : data[1]) {
        c = rankChar(n->rank);
        cout << " " << c;
    }
    cout << "\nYour hand:";
    for (auto n : data[5]) {
        c = rankChar(n->rank);
        cout << " "  << c << n->type;
    }
    cout << "\nLegal plays:";
    for (auto n : data[6]) {
        c = rankChar(n->rank);
        cout << " "  << c << n->type;
    }
    cout << endl;

    if (dataSize == 7 || dataSize == 8) {  // if player turn
        cout << ">";
        return 1;
    }

    if (dataSize == 9 || dataSize == 10) {  // if computer turn
        cout << ">Player" << data[7][2]->rank;
        if (data[7][1]->rank) {
            cout << " plays ";
        } else {
            cout << " discards ";
        }
        c = rankChar(data[7][0]->rank);
        cout << c << data[7][0]->type << endl;
        return 0;
    }
    return 0;
}

void Text::modify(string type, string card) {
    if (type == "play") {
        cout << "Player" << subject->game->curPlayer << " plays " << card << ".\n";
    } else if (type == "discard") {
        cout << "Player" << subject->game->curPlayer << " discards " << card << ".\n";
    } else if (type == "ragequit") {
        cout << "Player" << subject->game->curPlayer 
                << " ragequits. A computer will now take over.\n";
    } else if (type == "deck") {
        char c;
        int n;
        for (int i = 0; i < 4; i++) {
            for (int j = 1; j <= 13; j++) {
                n = i * 13 + j;
                c = rankChar(subject->game->deck[n - 1]->rank);
                if (n != 1 && n != 14 && n != 27 && n != 40) {
                    cout << " ";
                }
                cout << c << subject->game->deck[n - 1]->type;
            }
            cout << endl;
        }
        cout << ">";
    }
}