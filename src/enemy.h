#ifndef ENEMY_H
#define ENEMY_H

#include <ncurses.h>
#include "gameObject.h"

class Enemy : public GameObject {
  public:
    Enemy(WINDOW *w, char c, int color, int pX, int pY);
    void update(int pX, int pY, bool move);
    void spawn();

  private:
    int distance();
    int dir;
    int playerX, playerY;
};

#endif
