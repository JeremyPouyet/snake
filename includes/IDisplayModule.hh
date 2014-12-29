#pragma once

#include <utility>
#include <list>
#include <string>
#include "Game.hh"

class IDisplayModule
{
public:

  typedef const std::list<game::position> &	pos_list;
  typedef const game::position &		pos;

  virtual ~IDisplayModule() {}
  virtual std::string const &	getName() const = 0;
  virtual int			init(const int &x, const int &y) = 0;
  virtual game::keys		play() = 0;
  virtual void                  refresh(pos_list snake, pos fruit, pos_list walls, const int &eaten_fruits, const game::direction &dir) = 0;
  virtual void			pause() = 0;
  virtual int                   dispScore(const int &eaten_fruits) = 0;
  virtual int			end_screen() = 0;
  virtual int			stop() = 0;

  virtual int			getX() const = 0;
  virtual int			getY() const = 0;
};
