#include <ncurses.h>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <unistd.h>

#include "gameObject.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "saber.h"
#include "constants.h"

using namespace std;

WINDOW* init();
bool collision(GameObject i, GameObject j);

int main() {
  bool gameOver = false;
  bool lost = false;
  int score = 0;
  int reload_count = 1000;
  int b_clock=0;
  int e_clock=0;

  int xMax, yMax;

  // init ncurses
  WINDOW *win = init();
  void endGameScreen(bool lost, int score);
  void exitGame(WINDOW *endScreen, int score);
  void lostGame(WINDOW *endScreen, int score);
  bool move(void);
  getmaxyx(win, yMax, xMax);
  refresh();

  // init player and a bullet object
  Player p(win, '@', PLAYER_PAIR, 32, 16);
  Bullet b(win, '*', BULLET_PAIR);
  Saber s(win, '/',  PLAYER_PAIR);

  // init array of enemies
  vector<Enemy> enemies;
  enemies.reserve(5);
  for (int i=0; i<5; ++i) enemies.push_back(Enemy(win, ENEMY_CHAR, ENEMY_PAIR, p.getX(), p.getY()));

  int c = wgetch(win);

  // game loop
  while(!gameOver) {
    // get input
    c = wgetch(win);

    // update if relevant
    if (c!=ERR) p.update(c);
    if (c == 'x') gameOver = true;
    if (c == 'p' and b.getActive() and !b.getFired() and reload_count > 400) b.setFired(true), reload_count=0, b.setX(p.getX()), b.setY(p.getY()), b.setDir(p.getDir());

    if (e_clock > 100-score)
    {
      e_clock = 0;
      for (int i=0; i<enemies.size(); ++i) {
        enemies[i].update(p.getX(), p.getY(), move());
      }
    }

    s.update(p.getX(), p.getY(), p.getDir());

    if (b_clock > 10)
    {
      b_clock = 0;
      b.update();
    }

    p.display();
    b.display();
    s.display();
    for (int i=0; i<enemies.size(); ++i) enemies[i].display();

    wattron(win, COLOR_PAIR(SCORE_PAIR));
    mvwprintw(win, 1, 30, "Score: %d", score);
    wattroff(win, COLOR_PAIR(SCORE_PAIR));

    if (collision(p, s)) s.setActive(true), b.setActive(false);

    // collision detection
    for (int i=0; i<enemies.size(); ++i) {
      if (collision(enemies[i], b)) score++, enemies[i].spawn(), b.setFired(false);
      if (collision(enemies[i], p)) gameOver=true, lost=true;
      if (collision(enemies[i], s)) score++, enemies[i].spawn();
    }

    wrefresh(win);

    b_clock++;
    e_clock++;
    reload_count++;
  }

  wclear(win);
  wrefresh(win);

  // display end endScreen
  endGameScreen(lost, score);

  return 0;
}

WINDOW* init() {
  srand(time(NULL));
  initscr();
  WINDOW *win = newwin(32, 64, 0, 0);
  start_color();

  // init colors
  init_pair(BULLET_PAIR, COLOR_CYAN, COLOR_BLACK);
  init_pair(SCORE_PAIR, COLOR_GREEN, COLOR_BLACK);
  init_pair(PLAYER_PAIR, COLOR_YELLOW, COLOR_BLACK);
  init_pair(ENEMY_PAIR, COLOR_RED, COLOR_BLACK);
  init_pair(END_PAIR, COLOR_BLACK, COLOR_RED);

  wtimeout(win, 1);
  //wrefresh(win);
  box(win, 0, 0);
  mvwhline(win, 3, 1, 0, 62);

  curs_set(0);
  noecho();

  return win;
}

bool collision(GameObject i, GameObject j) {
  return (i.getX()==j.getX() && i.getY()==j.getY()) ? true : false;
}

bool move(void) { return (bool)(rand() % 2); }

void exitGame(WINDOW *endScreen, int score) {
  mvwprintw(endScreen, 4, 33, "                     ");
  mvwprintw(endScreen, 5, 32, "    ---------------    ");
  mvwprintw(endScreen, 6, 40, "Score: %d", score);
  mvwprintw(endScreen, 7, 32, "    ---------------    ");
  mvwprintw(endScreen, 8, 33, "                     ");
  mvwprintw(endScreen, 14, 33, "                     ");
  mvwprintw(endScreen, 15, 32, "    ---------------    ");
  mvwprintw(endScreen, 16, 32, "    |  EXIT GAME  |    ");
  mvwprintw(endScreen, 17, 32, "    ---------------    ");
  mvwprintw(endScreen, 18, 33, "                     ");
}

void lostGame(WINDOW *endScreen, int score) {
  mvwprintw(endScreen, 4, 33, "                     ");
  mvwprintw(endScreen, 5, 32, "    ---------------    ");
  mvwprintw(endScreen, 6, 40, "Score: %d", score);
  mvwprintw(endScreen, 7, 32, "    ---------------    ");
  mvwprintw(endScreen, 8, 33, "                     ");
  mvwprintw(endScreen, 14, 33, "                     ");
  mvwprintw(endScreen, 15, 32, "    ---------------    ");
  mvwprintw(endScreen, 16, 32, "    |  Lost Game  |    ");
  mvwprintw(endScreen, 17, 32, "    ---------------    ");
  mvwprintw(endScreen, 18, 33, "                     ");
}

void endGameScreen(bool lost, int score) {
  WINDOW *endScreen = newwin(32, 64, 0, 0);

  wattron(endScreen, COLOR_PAIR(END_PAIR));
  lost ? lostGame(endScreen, score) : exitGame(endScreen, score);
  wattroff(endScreen, COLOR_PAIR(1));
  wtimeout(endScreen, 1);
  wrefresh(endScreen);

  // sleep for 3 seconds
  usleep(3000000);

  box(endScreen, 0, 0);
  curs_set(0);
  noecho();

  endwin();
}
