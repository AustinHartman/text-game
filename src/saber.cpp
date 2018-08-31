#include "saber.h"

Saber::Saber(WINDOW *w, char c, int color) : GameObject(w, c, color) {
  // random spawn location on screen
  xLoc = rand() % (xMax-3) + 2;
  yLoc = rand() % (yMax-6) + 4;
}

void Saber::setActive(bool a) {
  active = a;
}

bool Saber::getActive() const {
  return active;
}

void Saber::update(int pX, int pY, int pDir) {
  prevX = xLoc;
  prevY = yLoc;

  // sword must be weilded
  if (active) {
    switch (pDir) {
      case 0:
        xLoc=pX, yLoc=pY+1;
        break;
      case 1:
        xLoc=pX, yLoc=pY-1;
        break;
      case 2:
        xLoc=pX-1, yLoc=pY;
        break;
      case 3:
        xLoc=pX+1, yLoc=pY;
        break;
      default:
        break;
    }
  }
}
