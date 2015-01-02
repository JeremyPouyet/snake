#pragma once

#include <sstream>
#include <string>
#include <list>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <map>

#define MAX_X 48
#define MAX_Y 26
#define MIN_X 15
#define MIN_Y 15
#define START_SPEED 8

namespace	game
{
  enum		keys
  {
    NAK = 0,	K_LEFT,
    K_RIGHT,	K_QUIT,
    K_PAUSE,	K_REPLAY,
    K_SWITCH,	K_PLUS,
    K_MINUS
  };

  enum		direction
  {
    NAD = 0,	D_RIGHT,
    D_UP,	D_LEFT,
    D_DOWN
  };

  struct	position
  {
    int	x;
    int	y;
    position(int x = 0, int y = 0);
  };

  struct	move
  {
    direction	dir;
    int		x;
    int		y;
  };

  class		snake
  {
  public:
    snake();
    int			init(const std::string &x, const std::string &y);
    bool		finish(keys k);
    void		action(keys k);
    int			getEatenFruits() const;
    direction		getDirection() const;
    void		reload();
    void		changeSpeed(int val);

    std::list<position>	get_snake_pos() const;
    position		get_fruit() const;
    int			getXmax() const;
    int			getYmax() const;
    std::list<position>	get_walls() const;
    int			getSpeed() const;
    ~snake() {}
  private:
    bool		is_snake(int x, int y) const;
    bool		is_wall(int x, int y) const;
    bool		is_fruit(int x, int y) const;
    void		generate_fruit();
    void		generate_wall();
    void                generatePos();
    void		move(direction d);

    std::list<position>	_snake_pos;
    position		_fruit;
    std::list<position>	_wall_pos;
    int			_Xmax;
    int			_Ymax;
    direction		_dir;
    int			_eaten_fruits;
    int			_speed;
  };
}
