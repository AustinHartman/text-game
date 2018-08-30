#include "gameObject.h"

GameObject::GameObject(WINDOW *w, char c, int color) {
  curwin = w;
  getmaxyx(curwin, yMax, xMax);
  character = c;
  print_color = color;
  xLoc = -1;
  yLoc = -1;
  yMin=5;
  xMin=2;
  prevX = -1;
  prevY = -1;
}

GameObject::GameObject(WINDOW *w, char c, int color, int x, int y) {
  curwin = w;
  getmaxyx(curwin, yMax, xMax);
  character = c;
  print_color = color;
  xLoc = x;
  yLoc = y;
  yMin=5;
  xMin=2;
  prevX = -1;
  prevY = -1;
}

void GameObject::setX(int x) { xLoc = x; }

void GameObject::setY(int y) { yLoc = y; }

int GameObject::getX() { return xLoc; }

int GameObject::getY() { return yLoc; }

void GameObject::setChar(char c) { character = c; }

void GameObject::display() {
  mvwaddch(curwin, prevY, prevX, ' ');
  wattron(curwin, COLOR_PAIR(print_color));
  mvwaddch(curwin, yLoc, xLoc, character);
  wattroff(curwin, COLOR_PAIR(print_color));
}
