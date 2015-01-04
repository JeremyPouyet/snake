#pragma once

#include <dlfcn.h>
#include <string>
#include <iostream>
#include "DisplayModule.hh"

class DLLoader
{
public:
  typedef IDisplayModule *(*entry)();
  DLLoader();
  int	init(const std::string &name);
  IDisplayModule *getInstance();
  ~DLLoader();
private:
  std::string	_name;
  void		*_entry_point;
};
