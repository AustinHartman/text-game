#ifndef BULLET_H
#define BULLET_H

#include "gameObject.h"
#include <ncurses.h>

class Bullet : public GameObject {
  public:
    Bullet(WINDOW *w, char c, int color);
    //Bullet(WINDOW *win, char c, int y, int x, int d, int color);
    void update();

    void setDir(int d);
    void setWin(WINDOW *win);
    void setChar(char c);
    void setFired(bool f);
    int getDir();
    bool getFired();
    bool getActive();
    void setActive(bool a);

  private:
    int dir;
    bool active, fired;
};

#endif
