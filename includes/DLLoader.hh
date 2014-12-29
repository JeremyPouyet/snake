#pragma once

#include <dlfcn.h>
#include <string>
#include <iostream>
#include "IDisplayModule.hh"

class DLLoader
{
public:
  typedef IDisplayModule *(*entry)(void);
  DLLoader();
  int	init(const std::string &name);
  IDisplayModule *getInstance();
  ~DLLoader();
private:
  std::string		name;
  void			*entry_point;
};
