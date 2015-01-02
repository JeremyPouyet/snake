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
  return playGame(mod, play, switcher);
}

int		playGame(IDisplayModule *mod, game::snake &play, LibSwitcher &switcher)
{
  game::keys	key;
  clock_t	init;
  bool		replay;

  do
  {
    do
    {
      mod->refresh(play.get_snake_pos(), play.get_fruit(), play.get_walls(), play.getEatenFruits(), play.getDirection());
      init = std::clock();
      do
	key = mod->play();
      while (key == game::NAK && (init - std::clock()) / play.getSpeed() > -1);
      play.action(key);
      if (key == game::K_PAUSE)
	mod->pause();
      else if (key == game::K_SWITCH)
	if (!(mod = switcher.change(mod)))
	  return -1;
    } while (!play.finish(key));
    replay = false;
    if (key != game::K_QUIT)
      if (mod->end_screen() == 1)
      {
	replay = true;
	play.reload();
      }
  } while (replay);
  switcher.stop(mod);
  return (0);
}
