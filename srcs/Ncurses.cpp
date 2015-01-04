#include "Ncurses.hh"

extern "C" IDisplayModule	*entry_point()
{
  return (new NcursesModule());
}

NcursesModule::NcursesModule() :
  ADisplayModule("lib_nibbler_Ncurses.so")
{ }

bool		NcursesModule::init(int x, int y)
{
  _x = x;
  _y = y;
  init_keys();
  if (initscr() == NULL)
    return false;
  if (start_color() == ERR)
    return false;
  if (use_default_colors() != ERR)
    {
      if (init_pair(1, COLOR_GREEN, -1) == ERR)
	return false;
      if (init_pair(2, COLOR_RED, -1) == ERR)
	return false;
      if (init_pair(3, COLOR_YELLOW, -1) == ERR)
	return false;
    }
  _height = ((LINES / 2) - (_y / 2));
  _width = ((COLS / 2) - (_x / 2));
  if (curs_set(0) == ERR)
    return false;
  if (nodelay(stdscr, TRUE) == ERR)
    return false;
  if (keypad(stdscr, TRUE) == ERR)
    return false;
  if (noecho() == ERR)
    return false;
  if ((_screen = newwin(_y + 2, _x + 2, _height, _width)) == NULL)
    return false;
  return true;
}

void	NcursesModule::init_keys()
{
  _key_map[KEY_LEFT] = K_LEFT;
  _key_map[KEY_RIGHT] = K_RIGHT;
  _key_map[KEY_PAUSE_MAJ] = K_PAUSE;
  _key_map[KEY_PAUSE_MIN] = K_PAUSE;
  _key_map[KEY_SWITCH_MAJ] = K_SWITCH;
  _key_map[KEY_SWITCH_MIN] = K_SWITCH;
  _key_map[KEY_PLUS] = K_PLUS;
  _key_map[KEY_MINUS] = K_MINUS;
  _key_map[KEY_ESC] = K_QUIT;
}

Keys	NcursesModule::play()
{
  int	key;

  if ((key = getch()) == ERR)
    return (Keys)0;
  return _key_map[key];
}

void    NcursesModule::refresh(const pos_list &snake, const pos &fruit, const pos_list &walls, int eaten_fruits, Direction dir)
{
  int	i, n;

  (void)dir;
  for (i = 1; i < _x + 1; i++)
    {
      for (n = 0; n < _y + 1; n++)
	mvwaddch(_screen, n, i, ' ');
      mvwaddch(_screen, n, i, '\n');
    }
  if (dispSnake(snake) == -1)
    return ;
  if (dispWalls(walls) == -1)
    return ;
  if (dispFruits(fruit) == -1)
    return ;
  if (box(_screen, 0, 0) != OK)
    return ;
  dispScore(eaten_fruits);
  if (wrefresh(_screen) == ERR)
    return ;
}

int             NcursesModule::dispScore(int eaten_fruits)
{
  mvwprintw(_screen, 0, 2, " Score : %d ", eaten_fruits * 10);
  return (0);
}

int	NcursesModule::dispSnake(pos_list snake) const
{
  for (auto it = snake.begin(); it != snake.end(); it++)
    {
      if (wattron(_screen, COLOR_PAIR(1)) == ERR)
	return -1;
	mvwaddch(_screen, (*it).y + 1, (*it).x + 1, (it == snake.begin()) ? '0' : 'O');
      if (wattroff(_screen, COLOR_PAIR(1)) == ERR)
	return -1;
    }
  return 0;
}

int	NcursesModule::dispFruits(const Position &fruit) const
{
  if (wattron(_screen, COLOR_PAIR(2)) == ERR)
    return -1;
  mvwaddch(_screen, fruit.y + 1, fruit.x + 1, '$');
  if (wattroff(_screen, COLOR_PAIR(2)) == ERR)
    return -1;
  return (0);
}

int	NcursesModule::dispWalls(const pos_list &walls) const
{
  for (auto it = walls.begin(); it != walls.end(); it++)
    {
      if (wattron(_screen, COLOR_PAIR(3)) == ERR)
	return -1;
      mvwaddch(_screen, (*it).y + 1, (*it).x + 1, '#');
      if (wattroff(_screen, COLOR_PAIR(3)) == ERR)
	return -1;
    }
  return 0;
}

void	NcursesModule::pause()
{
  int	key;

  if ((_menu = newwin(5, 25, ((LINES / 2) - (3 / 2)), ((COLS / 2) - (23 / 2)))) == NULL)
    return ;
  if (box(_menu, 0, 0) != OK)
    return ;
  mvwprintw(_menu, 1, 9, " Pause ");
  mvwprintw(_menu, 3, 2, " Press P to continue ");
  do
  {
    if (wrefresh(_menu) == ERR)
      return ;
    key = getch();
  } while (key != 80 && key != 112);
  if (wclear(_menu) == ERR)
    return ;
  if (wrefresh(_menu) == ERR)
    return ;
  if (delwin(_menu) == ERR)
    return ;
  if (endwin() == ERR)
    return ;
}

bool    NcursesModule::stop()
{
  if (echo() == ERR)
    return false;
  if (delwin(_screen) == ERR)
    return false;
  return !(endwin() == ERR);
}

int	NcursesModule::end_screen()
{
  int	key;

  if ((_menu = newwin(5, 15, ((LINES / 2) - (3 / 2)), ((COLS / 2) - (13 / 2)))) == NULL)
    return 0;
  if (box(_menu, 0, 0) != OK)
    return 0;
  mvwprintw(_menu, 1, 2, " Game Over ");
  mvwprintw(_menu, 3, 2, "  Retry ? ");
  do
  {
    if (wrefresh(_menu) == ERR)
      return 0;
    key = getch();
  } while (key != 121 && key != 89 && key != 110 && key != 78 && key != 27);
  if (key == 89 || key == 121)
    return !(delwin(_menu) == ERR);
  else if (key == 78 || key == 110 || key == 27)
    if (delwin(_menu) == ERR)
      return 0;
  return 0;
}
