//
// main.cpp for tp in /home/pouyet_j/test
//
// Made by Jérémy POUYET
// Login   <pouyet_j@epitech.net>
//
// Started on  Sat Mar 15 16:03:59 2014 Jérémy POUYET
// Last update Sun Apr  6 16:01:43 2014 Jérémy POUYET
//

#include <iostream>
#include <string>
#include <ctime>
#include "LibSwitcher.hh"

int            playGame(IDisplayModule *mod, game::snake &play, LibSwitcher &switcher);

int				main(int ac, char **av)
{
  IDisplayModule		*mod;
  LibSwitcher			switcher;
  game::snake			play;

  if (ac != 4)
    {
      std::cerr << "usage : ./nibller X Y lib" << std::endl;
      return -1;
    }
  if (!(mod = switcher.load(av[3])))
    return -1;
  if (!play.init(av[1], av[2]))
    return (-1);
  if (!mod->init(play.getXmax(), play.getYmax()))
    return (-1);
  if (playGame(mod, play, switcher) == -1)
    return (-1);
  return 1;
}

int		playGame(IDisplayModule *mod, game::snake &play, LibSwitcher &switcher)
{
  game::keys	key;
  clock_t	init;
  int		replay = 1;

  while (replay)
     {
      while (!play.finish(key))
	{
	  key = game::NAK;
	  mod->refresh(play.get_snake_pos(), play.get_fruit(), play.get_walls(), play.getEatenFruits(), play.getDirection());
	  init = std::clock();
	  while (key == game::NAK && (init - std::clock()) / play.getSpeed() > -1)
	    key = mod->play();
	  play.action(key);
	  if (key == game::K_PAUSE)
	    mod->pause();
	  else if (key == game::K_SWITCH)
	    if (!(mod = switcher.change(mod)))
	      return -1;
	}
      replay = 0;
      if (key != game::K_QUIT)
	if (mod->end_screen() == 1)
	  {
	    replay = 1;
	    play.reload();
	  }
     }
  switcher.stop(mod);
  return (0);
}
