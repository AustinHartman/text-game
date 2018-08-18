#include "enemy.h"
#include "constants.h"
#include <cstdlib>
#include <math.h>

Enemy::Enemy(WINDOW *w, char c, int color, int pX, int pY) : GameObject(w, c, color) {
  playerX = pX;
  playerY = pY;
  spawn();
}

// function will be used within spawn func to make sure spawns do not occur too close to player
int Enemy::distance() {
  return sqrt (pow(playerX-xLoc, 2) + pow(playerY-yLoc, 2));
}

void Enemy::update(int pX, int pY, bool move) {
  prevX = xLoc;
  prevY = yLoc;

  playerX = pX;
  playerY = pY;

  // AI so that enemy tracks player
  if (move) {
    dir = rand() % 2;
    if (xLoc > playerX and dir == 1) xLoc--;
    else if (xLoc < playerX and dir == 1) xLoc++;

    if (yLoc > playerY and dir == 0) yLoc--;
    else if (yLoc < playerY and dir == 0) yLoc++;
  }
  /*
  switch(dir) {
    case 0:
      if (yLoc <= yMax-3) yLoc++;
      break;
    case 1:
      if (yLoc >= yMin) yLoc--;
      break;
    case 2:
      if (xLoc >= xMin) xLoc--;
      break;
    case 3:
      if (xLoc <= xMax-3) xLoc++;
      break;
    default:
      break;
  }
  */
}

void Enemy::spawn() {
  dir = rand() % 4;

  xLoc = rand() % (xMax-3) + 2;
  yLoc = rand() % (yMax-6) + 4;
  if (distance() < 10) spawn();
}
