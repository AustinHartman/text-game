#include "player.h"
#include "constants.h"

Player::Player(WINDOW *w, char c, int color, int x, int y) : GameObject(w, c, color, x, y)
{
  curwin = w;
  getmaxyx(curwin, yMax, xMax);
  character = c;
  print_color = color;
  dir = 1;
  keypad(curwin, true);
}

int Player::getDir() { return dir; }

void Player::setBounds(int xMax, int xMin, int yMax, int yMin) {
  this->xMax = xMax;
  this->xMin = xMin;
  this->yMax = yMax;
  this->yMin = yMin;
}

void Player::update(int choice) {
  prevX = xLoc;
  prevY = yLoc;

  switch(choice) {
    case 'w':
      if (yLoc >= yMin) yLoc--;
      dir = 1;
      break;
    case 's':
      if (yLoc <= yMax-3) yLoc++;;
      dir = 0;
      break;
    case 'a':
      if (xLoc >= xMin) xLoc--;
      dir = 2;
      break;
    case 'd':
      if (xLoc <= xMax-3) xLoc++;
      dir = 3;
      break;
    default:
      break;
  }
}
