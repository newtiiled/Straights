#ifndef GAMESES_H
#define GAMESES_H

#include <vector>
#include <memory>
using namespace std;

class Player;
class Card;

struct GameSession {
    unsigned seed;
    int curPlayer; // the current player # (1-4)
    vector<char> types;
    vector<unique_ptr<Player>> players;
    vector<shared_ptr<Card>> deck;  // org shuffled deck
    vector<vector<shared_ptr<Card>>> table;  // cards on table
    vector<shared_ptr<Card>> allLegal;  // legal cards that can be played
    shared_ptr<Card> curCard; // card just played
    int playType = 1; // type of play just played, 0 discard, 1 play
    vector<int> oldScores;
    vector<int> winner; // winners, empty if no winners found (not end game)

  public:
    GameSession(unsigned seed, char one, char two, char three, char four);
    ~GameSession();

    void newRound();
    void shuffleDeck();
    vector<shared_ptr<Card>> legalPlays();
    void nextPlay(); // played by comp only, updates allLegal
    void nextPlayer();
    void findWinner();
    shared_ptr<Card> findCard(int rank, char type);

    void playCard(shared_ptr<Card> card);        // use exceptions
    void discardCard(shared_ptr<Card> card);
    void ragequit();

  private:
    void play(shared_ptr<Card> card);     // play update allLegal
    void discard(shared_ptr<Card> card);
};

#endif