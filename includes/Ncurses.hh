#pragma once

#include <iostream>
#include <ncurses.h>
#include <map>
#include "DisplayModule.hh"

#define	KEY_ESC 27
#define KEY_PAUSE_MAJ 80
#define KEY_PAUSE_MIN 112
#define KEY_SWITCH_MAJ 115
#define KEY_SWITCH_MIN 83
#define KEY_PLUS 43
#define KEY_MINUS 45

class NcursesModule : public ADisplayModule
{
public:
  NcursesModule();
  ~NcursesModule() {}
  bool	init(int x, int y);
  Keys	play();
  void  refresh(const pos_list &snake, const pos &fruit, const pos_list &walls, int eaten_fruits, Direction dir);
  void	pause();
  int	dispScore(int eaten_fruits);
  int	end_screen();
  bool	stop();
private:
  int     dispSnake(pos_list snake) const;
  int     dispFruits(const Position &fruit) const;
  int     dispWalls(const pos_list &walls) const;
  void	  init_keys();

  std::map<int, Keys>	_key_map;
  std::string			_name;
  WINDOW			*_screen;
  WINDOW			*_menu;
  int				_height;
  int				_width;
};
