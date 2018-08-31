#ifndef SABER_H
#define SABER_H

#include <ncurses.h>
#include <cstdlib>

#include "gameObject.h"

class Saber : public GameObject {
  public:
    Saber(WINDOW *w, char c, int color);

    void setActive(bool a);
    void update(int pX, int pY, int pDir);

    bool getActive() const;

  private:
    bool active;
};

#endif
