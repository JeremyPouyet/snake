#pragma once

#include <sstream>
#include <string>
#include <list>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <map>
#include <string>
#include <ctime>
#include "Items.hh"
#include "LibSwitcher.hh"

#define MAX_X 48
#define MAX_Y 26
#define MIN_X 15
#define MIN_Y 15
#define START_SPEED 8

class		snake
{
public:
  snake(char **av);
  int		init(const std::string &x, const std::string &y);
  bool		finish(Keys k);
  void		action(Keys k);
  int		getEatenFruits() const;
  Direction	getDirection() const;
  void		reload();
  void		changeSpeed(int val);
  int		play();

  std::list<Position>	get_snake_pos() const;
  Position		get_fruit() const;
  int			getXmax() const;
  int			getYmax() const;
  std::list<Position>	get_walls() const;
  int			getSpeed() const;
  ~snake() {}
private:
  bool		is_snake(int x, int y) const;
  bool		is_wall(int x, int y) const;
  bool		is_fruit(int x, int y) const;
  void		generate_fruit();
  void		generate_wall();
  void          generatePos();
  void		move(Direction d);

  std::list<Position>	_snake_pos;
  Position		_fruit;
  std::list<Position>	_wall_pos;
  int			_Xmax;
  int			_Ymax;
  Direction		_dir;
  int			_eaten_fruits;
  int			_speed;
  IDisplayModule	*_mod;
  LibSwitcher		_switcher;
};
