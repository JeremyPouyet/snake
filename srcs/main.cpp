#include "Game.hh"

int	main(int ac, char **av)
{
  if (ac != 4)
  {
    std::cerr << "usage : ./nibller X Y lib" << std::endl;
    return -1;
  }
  try {
    snake	game(av);
    return game.play();
  }
  catch (int e) {
      std::cerr << "error while creating game" << std::endl;
      return 1;
  }
  return 2;
}
