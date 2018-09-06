#include <ncurses.h>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sstream>
#include <fstream>

#include "gameObject.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "saber.h"
#include "constants.h"

using namespace std;

WINDOW* init();
bool collision(GameObject i, GameObject j);
void endGameScreen(bool lost, int score);
void exitGame(WINDOW *endScreen, int score);
void lostGame(WINDOW *endScreen, int score);
bool move(void);
bool mainMenu();
int getHighscore(string file);
void clearFile(string filename);
void updateHighscore(string file, int score);


int main() {
  bool gameOver = false;
  bool lost = false;
  bool saberFound = false;
  int score = 0;
  int hs = getHighscore("highscore.txt");
  int reload_count = 1000;
  int b_clock=0;
  int e_clock=0;
  int xMax, yMax;

  // init ncurses
  WINDOW *win = init();
  gameOver = mainMenu();
  getmaxyx(win, yMax, xMax);
  refresh();

  // init player and a bullet object
  Player p(win, '@', PLAYER_PAIR, 32, 16);
  Bullet b(win, '*', BULLET_PAIR);
  Saber s(win, '/',  PLAYER_PAIR);

  // init grass objects
  vector<GameObject> grass;
  grass.reserve(GRASS_COUNT);
  for (int i=0; i<GRASS_COUNT; ++i) grass.push_back(GameObject(win, '^', SCORE_PAIR, rand() % (xMax) + 1, rand() % (yMax-5) + 4));

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
    if      (c == 'x') gameOver = true;
    else if (c == 'p' and b.getActive() and !b.getFired() and reload_count > 400) b.setFired(true), reload_count=0, b.setX(p.getX()), b.setY(p.getY()), b.setDir(p.getDir());
    // weapon change
    else if (saberFound and c == 'c') {
      if (b.getActive() and p.getX()<xMax-2 and p.getX()>1 and p.getY()<yMax-2 and p.getY()>4) {
        b.setActive(false);
        s.setActive(true);
        s.setChar('/');
        p.setBounds(xMax-1, 3, yMax-1, 6);
      }
      else {
        b.setActive(true);
        s.setActive(false);
        s.setChar(' ');
        p.setBounds(xMax, 2, yMax, 5);
      }
    }

    if (e_clock > 100-score)     // enemies speed up as score goes up
    {
      e_clock = 0;
      for (int i=0; i<enemies.size(); ++i) {
        enemies[i].update(p.getX(), p.getY(), move());
      }
    }

    p.update(c);
    s.update(p.getX(), p.getY(), p.getDir());

    // can alter reload time for bullet
    if (b_clock > 10)
    {
      b_clock = 0;
      b.update();
    }

    // display objects
    s.display();            // should be displayed first bc saber is whitespace when unweilded
    p.display();
    b.display();

    for (int i=0; i<enemies.size(); ++i) enemies[i].display();
    for (int i=0; i<grass.size(); ++i) grass[i].display();


    wattron(win, COLOR_PAIR(SCORE_PAIR));
    mvwprintw(win, 1, 5, "Score: %d", score);
    mvwprintw(win, 2, 5, "Highscore: %d", hs);
    wattroff(win, COLOR_PAIR(SCORE_PAIR));

    wattron(win, COLOR_PAIR(CONTROL_PAIR));
    mvwprintw(win, 1, 25, " Move: wasd");
    mvwprintw(win, 2, 25, " Shoot: p  ");
    mvwprintw(win, 1, 45, " Exit: x        ");
    mvwprintw(win, 2, 45, " Swap weapon: c ");
    wattroff(win, COLOR_PAIR(CONTROL_PAIR));

    // set saber to active and change bounds so saber stays in bounds
    if (!saberFound && collision(p, s)) {
      saberFound = true;
      p.setBounds(xMax-1, 3, yMax-1, 6);
      s.setActive(true);
      b.setActive(false);      // once saber is picked up, gun is inactive
    }

    // collision detection
    for (int i=0; i<enemies.size(); ++i) {
      if (collision(enemies[i], b)) score++, enemies[i].spawn(), b.setFired(false);
      if (collision(enemies[i], p)) gameOver=true, lost=true;
      if (collision(enemies[i], s) && s.getActive()) score++, enemies[i].spawn();
    }

    wrefresh(win);

    b_clock++;
    e_clock++;
    reload_count++;
  }

  // erase window from mem
  delwin(win);

  // display end endScreen
  endGameScreen(lost, score);

  // update highscore
  updateHighscore("highscore.txt", score);

  return 0;
}

int getHighscore(string filename) {
  string line;
  string hs_str;
  ifstream file (filename);
  if (file.is_open()) {
    while ( getline (file, line) ) hs_str = line;
    file.close();
  }

  // convert str to int
  stringstream conv((hs_str));
  int hs;
  conv >> hs;

  return hs;
}

void clearFile(string filename) {
  ofstream ofs;
  ofs.open(filename, std::ofstream::out | std::ofstream::trunc);
  ofs.close();
}

void updateHighscore(string filename, int score) {
  int current_hs = getHighscore(filename);
  if (score > current_hs) {
    clearFile(filename);

    ofstream file;
    file.open(filename);
    string s = to_string(score);
    file << score << "\n";
    file.close();
  }
}

bool mainMenu() {

  WINDOW *menuScreen = newwin(32, 64, 0, 0);
  wrefresh(menuScreen);
  wtimeout(menuScreen, 1);

  bool onMenu = true;
  bool play = true;
  int c;

  do {
    c = wgetch(menuScreen);
    if (c == 'w') play = true;
    else if (c == 's') play = false;
    else if (c == '\n') {
      onMenu = false;
      delwin(menuScreen);
      return (play) ? false : true;
    }

    wattron(menuScreen, COLOR_PAIR(END_PAIR));
    mvwprintw(menuScreen, 16, 32, "MAIN MENU");
    wattroff(menuScreen, COLOR_PAIR(END_PAIR));

    if (play) {
      wattron(menuScreen, COLOR_PAIR(END_PAIR));
      mvwprintw(menuScreen, 18, 32, "Play");
      wattroff(menuScreen, COLOR_PAIR(END_PAIR));

      wattron(menuScreen, COLOR_PAIR(ENEMY_PAIR));
      mvwprintw(menuScreen, 20, 32, "Exit");
      wattroff(menuScreen, COLOR_PAIR(ENEMY_PAIR));
    } else {
      wattron(menuScreen, COLOR_PAIR(ENEMY_PAIR));
      mvwprintw(menuScreen, 18, 32, "Play");
      wattroff(menuScreen, COLOR_PAIR(ENEMY_PAIR));

      wattron(menuScreen, COLOR_PAIR(END_PAIR));
      mvwprintw(menuScreen, 20, 32, "Exit");
      wattroff(menuScreen, COLOR_PAIR(END_PAIR));
    }
    wtimeout(menuScreen, 1);
    wrefresh(menuScreen);
  } while (onMenu);

  // probably will never reach here, but will remove compiler warning
  return false;
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
  init_pair(CONTROL_PAIR, COLOR_BLACK, COLOR_WHITE);

  wtimeout(win, 1);
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
  mvwprintw(endScreen, 5, 32, "    ---------------    ");
  mvwprintw(endScreen, 6, 40, "Score: %d", score);
  mvwprintw(endScreen, 7, 32, "    ---------------    ");

  wattron(endScreen, COLOR_PAIR(END_PAIR));
  mvwprintw(endScreen, 14, 33, "                     ");
  mvwprintw(endScreen, 15, 32, "    ---------------    ");
  mvwprintw(endScreen, 16, 32, "    |  EXIT GAME  |    ");
  mvwprintw(endScreen, 17, 32, "    ---------------    ");
  mvwprintw(endScreen, 18, 33, "                     ");
  wattroff(endScreen, COLOR_PAIR(1));
}

void lostGame(WINDOW *endScreen, int score) {
  mvwprintw(endScreen, 5, 32, "    ---------------    ");
  mvwprintw(endScreen, 6, 40, "Score: %d", score);
  mvwprintw(endScreen, 7, 32, "    ---------------    ");

  wattron(endScreen, COLOR_PAIR(END_PAIR));
  mvwprintw(endScreen, 14, 33, "                     ");
  mvwprintw(endScreen, 15, 32, "    ---------------    ");
  mvwprintw(endScreen, 16, 32, "    |  Lost Game  |    ");
  mvwprintw(endScreen, 17, 32, "    ---------------    ");
  mvwprintw(endScreen, 18, 33, "                     ");
  wattroff(endScreen, COLOR_PAIR(1));
}

void endGameScreen(bool lost, int score) {
  WINDOW *endScreen = newwin(32, 64, 0, 0);

  lost ? lostGame(endScreen, score) : exitGame(endScreen, score);

  wtimeout(endScreen, 1);
  wrefresh(endScreen);

  // sleep for 3 seconds
  usleep(3000000);

  box(endScreen, 0, 0);
  curs_set(0);
  noecho();

  endwin();
}
