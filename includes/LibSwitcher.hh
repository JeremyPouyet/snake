#pragma once

#include <string>
#include <list>
#include "DisplayModule.hh"
#include "DLLoader.hh"

class LibSwitcher
{
public:
  LibSwitcher();
  ~LibSwitcher() {}
  IDisplayModule	*load(const std::string &name);
  IDisplayModule	*change(IDisplayModule *mod);
  void			stop(IDisplayModule *mod);
private:

  std::list<std::string>	_libs;
  DLLoader			_loader;

  void	init_libs();
};
