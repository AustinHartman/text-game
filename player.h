#ifndef PLAYER_H
#define PLAYER_H

#include <ncurses.h>
#include "gameObject.h"

class Player : public GameObject {
  public:
    Player(WINDOW *w, char c, int color, int x, int y);
    void update(int choice);
    int getDir();

  private:
    int dir;
};

#endif
