#ifndef SUBJECT_H
#define SUBJECT_H

#include "observer.h"
#include <vector>
#include <memory>
using namespace std;

class Subject {
    vector<Observer *> observers;

  public:
    Subject();
    void attach(Observer *ob);
    void detach(Observer *ob);
    int notifyObservers();  // 0 if human, 1 if end game
    void modifyObservers(string type, string card);
    virtual ~Subject() = 0;
};

#endif