#pragma once

#include <iostream>
#include <ncurses.h>
#include <map>
#include "IDisplayModule.hh"

#define	KEY_ESC 27
#define KEY_PAUSE_MAJ 80
#define KEY_PAUSE_MIN 112
#define KEY_SWITCH_MAJ 115
#define KEY_SWITCH_MIN 83
#define KEY_PLUS 43
#define KEY_MINUS 45

class NcursesModule : public IDisplayModule
{
public:
  NcursesModule();
  virtual ~NcursesModule() {}
  virtual std::string const & getName() const;
  virtual int init(const int &x, const int &y);
  virtual game::keys            play();
  virtual void                  refresh(pos_list snake, pos fruit, pos_list walls, const int &eaten_fruits, const game::direction &dir);
  virtual void			pause();
  virtual int			dispScore(const int &eaten_fruits);
  virtual int			end_screen();
  virtual int			stop();
  virtual int			getX() const;
  virtual int			getY() const;
private:
  int     dispSnake(pos_list snake) const;
  int     dispFruits(const game::position &fruit) const;
  int     dispWalls(pos_list walls) const;
  void	  init_keys();

private:
  int x;
  int y;
  std::map<int, game::keys> key_map;
  std::string   name;
  WINDOW *screen;
  WINDOW *menu;
  int	height;
  int	width;
};
