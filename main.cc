#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <stdexcept>
#include "text.h"
using namespace std;

class InvalidPlay{};
class InvalidDiscard{};

int main( int argc, char * argv[] ) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    if ( argc > 1 ) {
		try {
			seed = std::stoi( std::string{ argv[1] } );
		} catch( std::invalid_argument & e ) {
			std::cerr << e.what() << std::endl;
			return 1;
		} catch( std::out_of_range & e ) {
			std::cerr << e.what() << std::endl;
			return -1;
		}
	}

    vector<char> types;
    char c;
    for (int i = 1; i <= 4; i++) {
        cout << "Is Player" << i << " a human (h) or computer (c)?\n>";
        cin >> c;
        types.push_back(c);
    }

    auto str = make_shared<Straights>(seed, types[0], types[1], types[2], types[3]);
    auto view = make_unique<Text>(str);

    string type;
    string card;
    int n = str->notifyObservers();
    bool goodInput = true;
    while (!n) {
        cin >> type;
        while (type != "quit" && type != "play" 
                && type != "discard" && type != "ragequit" 
                && type != "deck") {
            cout << ">";
            cin >> type;
        }
        if (type == "quit") {
            return 0;
        }
        if (type == "play" || type == "discard") {
            cin >> card;
            try {
                str->setState(type, card);
                goodInput = true;
            }
            catch (InvalidPlay) {
                cout << "This is not a legal play.\n>";
                goodInput = false;
            }
            catch (InvalidDiscard) {
                cout << "You have a legal play. You may not discard.\n>";
                goodInput = false;
            }
        }
        if (type == "deck") {
            str->setState(type);
            goodInput = false;
        }
        if (type == "ragequit") {
            str->setState(type);
            goodInput = true;
        }
        if (goodInput) {
            n = str->notifyObservers();
        }
    }

    return 0;
}