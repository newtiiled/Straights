#ifndef TEXT_H
#define TEXT_H

#include "observer.h"
#include "straights.h"
#include <vector>
#include <memory>
using namespace std;

class Text : public Observer {
    shared_ptr<Straights> subject;
    bool start;

  public:
    Text(shared_ptr<Straights> game);
    int notify() override;  // the first time, when start true, print new round
    void modify(string type, string card) override;
    ~Text();
  
  private:
    char rankChar(int n);
};

#endif