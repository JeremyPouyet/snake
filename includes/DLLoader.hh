//
// DLLoader.hh for tp in /home/pouyet_j/test
//
// Made by Jérémy POUYET
// Login   <pouyet_j@epitech.net>
//
// Started on  Sat Mar 15 15:17:09 2014 Jérémy POUYET
// Last update Sun Apr  6 09:41:25 2014 Jérémy POUYET
//

#ifndef	DLLOADER_HH_
# define DLLOADER_HH_

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

#endif
