//
// IDisplayModule.hh for tp in /home/pouyet_j/test
//
// Made by Jérémy POUYET
// Login   <pouyet_j@epitech.net>
//
// Started on  Sat Mar 15 13:49:59 2014 Jérémy POUYET
// Last update Sun Apr  6 15:58:26 2014 jerome campeaux
//

#ifndef	IDISPLAYMODULE_HH_
# define IDISPLAYMODULE_HH_

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

#endif
