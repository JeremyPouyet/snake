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
      NAK,
      K_LEFT,
      K_RIGHT,
      K_QUIT,
      K_PAUSE,
      K_REPLAY,
      K_SWITCH,
      K_PLUS,
      K_MINUS
    };

  enum		direction
    {
      NAD,
      D_RIGHT,
      D_UP,
      D_LEFT,
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
    int			finish(const keys &k);
    void		action(const keys &k);
    const int		&getEatenFruits() const;
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
    void		init_left();
    void		init_right();
    int			is_snake(const int &x, const int &y) const;
    int			is_wall(const int &x, const int &y) const;
    int			is_fruit(const int &x, const int &y) const;
    void		generate_fruit();
    void		generate_wall();
    void                generatePos();
    void		move(const direction &d);

    std::list<position>	snake_pos;
    position		fruit;
    std::list<position>	wall_pos;
    int			Xmax;
    int			Ymax;
    direction		dir;
    std::map<direction, direction> left;
    std::map<direction, direction> right;
    int			eaten_fruits;
    int			speed;
  };
}
