#ifndef OBS_H
#define OBS_H
#include <memory>
#include <string>

class Observer {
  public:
    virtual int notify() = 0; // 0 if comp, 1 if human, 2 if end game
    virtual void modify(std::string type, std::string card) = 0;
    virtual ~Observer();
};

#endif