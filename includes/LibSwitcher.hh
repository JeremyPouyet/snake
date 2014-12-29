//
// LibSwitcher.hh for nibbler in /home/pouyet_j/rendu/cpp_nibbler
//
// Made by Jérémy POUYET
// Login   <pouyet_j@epitech.net>
//
// Started on  Sat Apr  5 12:53:55 2014 Jérémy POUYET
// Last update Sat Apr  5 14:13:24 2014 Jérémy POUYET
//

#ifndef LIBSWITCHER_HH_
# define LIBSWITCHER_HH_

#include <string>
#include <list>
#include "IDisplayModule.hh"
#include "DLLoader.hh"

class LibSwitcher
{
public:
  LibSwitcher();
  ~LibSwitcher();
  IDisplayModule	*load(const std::string &name);
  IDisplayModule	*change(IDisplayModule *mod);
  void			stop(IDisplayModule *mod);
private:

  std::list<std::string>	libs;
  DLLoader	loader;

  void	init_libs();
};

#endif
