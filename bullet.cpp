#include "bullet.h"
#include "constants.h"

Bullet::Bullet(WINDOW *w, char c, int color) : GameObject(w, c, color) {
  active = true;
  fired = false;
}
/*
Bullet::Bullet(WINDOW *win, char c, int y, int x, int d, int color) : GameObject(win, c, color) {
  yLoc = y;
  xLoc = x;
  dir = d;
}
*/
int Bullet::getDir() { return dir; }

bool Bullet::getFired() { return fired; }

bool Bullet::getActive() { return active; }

void Bullet::setDir(int d) { dir = d; }

void Bullet::setWin(WINDOW *w) { curwin = w; }

void Bullet::setChar(char c) { character = c; }

void Bullet::setFired(bool f) { fired = f; }

void Bullet::setActive(bool a) { active = a; }

void Bullet::update() {
  // 0=down, 1=up, 2=left, 3=right
  prevX = xLoc;
  prevY = yLoc;
  if (active and fired) {
    switch(dir) {
      case 0:
        if (yLoc <= yMax-3) yLoc++;
        else fired=false;
        break;
      case 1:
        if (yLoc >= yMin) yLoc--;
        else fired=false;
        break;
      case 2:
        if (xLoc >= xMin) xLoc--;
        else fired=false;
        break;
      case 3:
        if (xLoc <= xMax-3) xLoc++;
        else fired=false;
        break;
      default:
        break;
    }
  }
  else {
    yLoc = -1;
    xLoc = -1;
  }
}
