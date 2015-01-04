#include "Game.hh"

int	main(int ac, char **av)
{
  if (ac != 4)
  {
    std::cerr << "usage : ./nibller X Y lib" << std::endl;
    return -1;
  }
  snake	game(av);
  return game.play();
}
