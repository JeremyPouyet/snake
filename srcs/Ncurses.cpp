//
// amazing.cpp for super.c in /home/pouyet_j/test
//
// Made by Jérémy POUYET
// Login   <pouyet_j@epitech.net>
//
// Started on  Fri Mar 14 16:32:41 2014 Jérémy POUYET
// Last update Sun Apr  6 16:20:15 2014 Jérémy POUYET
//

#include "Ncurses.hh"

extern "C" IDisplayModule	*entry_point()
{
  return (new NcursesModule());
}

NcursesModule::NcursesModule() : name("lib_nibbler_Ncurses.so")
{

}

std::string const & NcursesModule::getName() const
{
  return this->name;
}

int		NcursesModule::init(const int &x, const int &y)
{
  this->x = x;
  this->y = y;
  this->init_keys();
  if (initscr() == NULL)
    return (0);
  if (start_color() == ERR)
    return (0);
  if (use_default_colors() != ERR)
    {
      if (init_pair(1, COLOR_GREEN, -1) == ERR)
	return (0);
      if (init_pair(2, COLOR_RED, -1) == ERR)
	return (0);
      if (init_pair(3, COLOR_YELLOW, -1) == ERR)
	return (0);
    }
  this->height = ((LINES / 2) - (this->y / 2));
  this->width = ((COLS / 2) - (this->x / 2));
  if (curs_set(0) == ERR)
    return (0);
  if (nodelay(stdscr, TRUE) == ERR)
    return (0);
  if (keypad(stdscr, TRUE) == ERR)
    return (0);
  if (noecho() == ERR)
    return (0);
  if ((this->screen = newwin(this->y + 2, this->x + 2, this->height, this->width)) == NULL)
    return (0);
  return (1);
}

void	NcursesModule::init_keys()
{
  this->key_map[KEY_LEFT] = game::K_LEFT;
  this->key_map[KEY_RIGHT] = game::K_RIGHT;
  this->key_map[KEY_PAUSE_MAJ] = game::K_PAUSE;
  this->key_map[KEY_PAUSE_MIN] = game::K_PAUSE;
  this->key_map[KEY_SWITCH_MAJ] = game::K_SWITCH;
  this->key_map[KEY_SWITCH_MIN] = game::K_SWITCH;
  this->key_map[KEY_PLUS] = game::K_PLUS;
  this->key_map[KEY_MINUS] = game::K_MINUS;
  this->key_map[KEY_ESC] = game::K_QUIT;
}

game::keys            NcursesModule::play()
{
  int	key;

  if ((key = getch()) == ERR)
    return (game::keys)0;
  return this->key_map[key];
}

void    NcursesModule::refresh(pos_list snake, pos fruit, pos_list walls, const int &eaten_fruits, const game::direction &dir)
{
  int	i = 1;
  int	n = 1;

  (void)dir;
  while (i < this->x + 1)
    {
      n = 0;
      while (n < this->y + 1)
	mvwaddch(this->screen, n++, i, ' ');
      mvwaddch(this->screen, n, i++, '\n');
    }
  if (dispSnake(snake) == -1)
    return ;
  if (dispWalls(walls) == -1)
    return ;
  if (dispFruits(fruit) == -1)
    return ;
  if (box(this->screen, 0, 0) != OK)
    return ;
  dispScore(eaten_fruits);
  if (wrefresh(this->screen) == ERR)
    return ;
}

int             NcursesModule::dispScore(const int &eaten_fruits)
{
  mvwprintw(this->screen, 0, 2, " Score : %d ", eaten_fruits * 10);
  return (0);
}

int	NcursesModule::dispSnake(pos_list snake) const
{
  for (std::list<game::position>::const_iterator it = snake.begin(); it != snake.end(); it++)
    {
      if (wattron(this->screen, COLOR_PAIR(1)) == ERR)
	return (-1);
      if (it == snake.begin())
	mvwaddch(this->screen, (*it).y + 1, (*it).x + 1, '0');
      else
	mvwaddch(this->screen, (*it).y + 1, (*it).x + 1, 'O');
      if (wattroff(this->screen, COLOR_PAIR(1)) == ERR)
	return (-1);
    }
  return (0);
}

int	NcursesModule::dispFruits(const game::position &fruit) const
{
  if (wattron(this->screen, COLOR_PAIR(2)) == ERR)
    return (-1);
  mvwaddch(this->screen, fruit.y + 1, fruit.x + 1, '$');
  if (wattroff(this->screen, COLOR_PAIR(2)) == ERR)
    return (-1);
  return (0);
}

int	NcursesModule::dispWalls(pos_list walls) const
{
  for (std::list<game::position>::const_iterator it = walls.begin(); it != walls.end(); it++)
    {
      if (wattron(this->screen, COLOR_PAIR(3)) == ERR)
	return (-1);
      mvwaddch(this->screen, (*it).y + 1, (*it).x + 1, '#');
      if (wattroff(this->screen, COLOR_PAIR(3)) == ERR)
	return (-1);
    }
  return (0);
}

void	NcursesModule::pause()
{
  int	key = 1;

  if ((this->menu = newwin(5, 25, ((LINES / 2) - (3 / 2)), ((COLS / 2) - (23 / 2)))) == NULL)
    return ;
  if (box(this->menu, 0, 0) != OK)
    return ;
  mvwprintw(this->menu, 1, 9, " Pause ");
  mvwprintw(this->menu, 3, 2, " Press P to continue ");
  while (key != 80 && key != 112)
    {
      if (wrefresh(this->menu) == ERR)
	return ;
      key = getch();
    }
  if (wclear(this->menu) == ERR)
    return ;
  if (wrefresh(this->menu) == ERR)
    return ;
  if (delwin(this->menu) == ERR)
    return ;
  if (endwin() == ERR)
    return ;
}

int    NcursesModule::stop()
{
  if (echo() == ERR)
    return (0);
  if (delwin(this->screen) == ERR)
    return (0);
  if (endwin() == ERR)
    return (0);
  return (1);
}

int	NcursesModule::end_screen()
{
  int	key = 1;

  if ((this->menu = newwin(5, 15, ((LINES / 2) - (3 / 2)), ((COLS / 2) - (13 / 2)))) == NULL)
    return (0);
  if (box(this->menu, 0, 0) != OK)
    return (0);
  mvwprintw(this->menu, 1, 2, " Game Over ");
  mvwprintw(this->menu, 3, 2, "  Retry ? ");
  while (key != 121 && key != 89 && key != 110 && key != 78 && key != 27)
    {
      if (wrefresh(this->menu) == ERR)
	return (0);
      key = getch();
    }
  if (key == 89 || key == 121)
    {
      if (delwin(this->menu) == ERR)
  	return (0);
      return (1);
    }
  else if (key == 78 || key == 110 || key == 27)
    if (delwin(this->menu) == ERR)
      return (0);
  return (0);
}

int	NcursesModule::getX() const
{
  return this->x;
}

int	NcursesModule::getY() const
{
  return this->y;
}
