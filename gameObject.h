#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <ncurses.h>

class GameObject {
  public:
    GameObject(WINDOW *w, char c, int color);
    GameObject(WINDOW *w, char c, int color, int x, int y);

    int getX();
    int getY();

    void setX(int x);
    void setY(int y);

    void display();

  protected:
    WINDOW *curwin;
    char character;
    int print_color;
    int xLoc, yLoc;
    int xMax, yMax;
    int xMin, yMin;
    int prevX, prevY;
};

#endif
