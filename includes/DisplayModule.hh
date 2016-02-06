#pragma once

#include <utility>
#include <list>
#include <string>
#include "Items.hh"

class IDisplayModule
{
public:

  typedef std::list<Position>	pos_list;
  typedef Position 		pos;

  virtual ~IDisplayModule() {}
  virtual std::string const &	getName() const = 0;
  virtual bool			init(int x, int y) = 0;
  virtual Keys			play() = 0;
  virtual void                  refresh(const pos_list &snake, const pos &fruit, const pos_list &walls, int eaten_fruits, Direction dir) = 0;
  virtual void			pause() = 0;
  virtual int                   dispScore(int eaten_fruits) = 0;
  virtual int			end_screen() = 0;
  virtual bool			stop() = 0;
  virtual int			getX() const = 0;
  virtual int			getY() const = 0;
};

class ADisplayModule : public IDisplayModule
{
public:
  ADisplayModule(const std::string &name) :
    _name(name)
    {}
  ~ADisplayModule()
    {}

  std::string const &	getName() const
    {
      return _name;
    }

  int			getX() const
    {
      return _x;
    }

  int			getY() const
    {
      return _y;
    }

protected:
  const std::string _name;
  int _x;
  int _y;
};
