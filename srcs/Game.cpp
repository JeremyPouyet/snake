#include "Game.hh"

static const Move          aod[] =
{
  {D_RIGHT, 1, 0},
  {D_UP, 0, -1},
  {D_LEFT, -1, 0},
  {D_DOWN, 0, 1},
  {NAD, NAD, NAD}
};

static const std::map<Direction, Direction> left =
{
  {D_RIGHT, D_UP},
  {D_UP, D_LEFT},
  {D_LEFT, D_DOWN},
  {D_DOWN, D_RIGHT}
};

static const std::map<Direction, Direction> right =
{
  {D_RIGHT, D_DOWN},
  {D_UP, D_RIGHT},
  {D_LEFT, D_UP},
  {D_DOWN, D_LEFT}
};

Position::Position(int x, int y) :
  x(x), y(y)
{ }

snake::snake(char **av) :
  _dir(D_RIGHT), _eaten_fruits(0), _speed(START_SPEED)
{
  if (!(_mod = _switcher.load(av[3])))
      throw -1;
  if (!init(av[1], av[2]))
    throw -1;
  if (!_mod->init(_Xmax, _Ymax))
    throw -1;
}

/*
** initialization functions
*/

int		snake::play()
{
  Keys		key;
  clock_t	init;
  bool		replay;

  do
  {
    do
    {
      _mod->refresh(_snake_pos, _fruit, _wall_pos, _eaten_fruits, _dir);
      init = std::clock();
      do
	key = _mod->play();
      while (key == NAK && (init - std::clock()) / getSpeed() > -1);
      action(key);
      if (key == K_PAUSE)
	_mod->pause();
      else if (key == K_SWITCH)
	if (!(_mod = _switcher.change(_mod)))
	  return -1;
    } while (!finish(key));
    replay = false;
    if (key != K_QUIT)
      if (_mod->end_screen() == 1)
      {
	replay = true;
	reload();
      }
  } while (replay);
  _switcher.stop(_mod);
  return (0);
}

int			snake::init(const std::string &x, const std::string &y)
{
  std::istringstream	convert;

  convert.str(x);
  convert >> _Xmax;
  convert.clear();
  convert.str(y);
  convert >> _Ymax;
  if (_Xmax > MAX_X || _Ymax > MAX_Y)
    {
      std::cerr << "Asked map is too big (Max : 48 * 26)" << std::endl;
      return 0;
    }
  if (_Xmax < MIN_Y || _Ymax < MIN_Y)
    {
      std::cerr << "Asked map is too short (Min : 15 * 15)" << std::endl;
      return 0;
    }
  generatePos();
  std::srand(time(NULL));
  generate_fruit();
  return 1;
}

void			snake::generatePos()
{
  for (int i = 0; i < 3; i++)
    _snake_pos.push_front({(_Xmax / 2) + i - 1, _Ymax / 2});
}

void			snake::reload()
{
  _snake_pos.clear();
  _wall_pos.clear();
  _eaten_fruits = 0;
  _speed = START_SPEED;
  _dir = D_RIGHT;
  generatePos();
  generate_fruit();
}

/*
** check functions
*/

bool	snake::is_snake(int x, int y) const
{
  for (auto it : _snake_pos)
    if (x == it.x && y == it.y)
      return true;
  return false;
}

bool	snake::is_wall(int x, int y) const
{
  for (auto it : _wall_pos)
    if (x == it.x && y == it.y)
      return true;
  return false;
}

bool	snake::is_fruit(int x, int y) const
{
  return (x == _fruit.x && y == _fruit.y);
}

/*
** generation functions
*/

void		snake::generate_fruit()
{
  do
  {
    _fruit.x = rand() % _Xmax;
    _fruit.y = rand() % _Ymax;
  } while (is_snake(_fruit.x, _fruit.y) || is_wall(_fruit.x, _fruit.y));
}

void		snake::generate_wall()
{
  Position	p;

  do
  {
    p.x = rand() % _Xmax;
    p.y = rand() % _Ymax;
  } while (is_snake(p.x, p.y) || is_fruit(p.x, p.y));
  _wall_pos.push_back(p);
}

/*
** game logic functions
*/

bool	snake::finish(Keys k)
{
  Position p = _snake_pos.front();

  if (p.x < 0 || p.x >= _Xmax || p.y < 0 || p.y >= _Ymax)
    return true;
  _snake_pos.pop_front();
  if (is_snake(p.x, p.y))
    return true;
  _snake_pos.push_front(p);
  return (is_wall(p.x, p.y) || k == K_QUIT);
}

void	snake::move(Direction d)
{
  Position	p, f = _snake_pos.front();
  Move	m;

  for (int i = 0; aod[i].dir != NAD; i++)
    if (aod[i].dir == d)
      {
	_dir = aod[i].dir;
	m = aod[i];
      }
  p.x = f.x += m.x;
  p.y = f.y += m.y;
  _snake_pos.push_front(p);
  if (is_fruit(p.x, p.y))
    {
      generate_fruit();
      if (++_eaten_fruits % 3 == 0)
	generate_wall();
    }
  else
    _snake_pos.pop_back();
}

void	snake::action(Keys k)
{
  if (k == K_LEFT)
    move(left.at(_dir));
  else if (k == K_RIGHT)
    move(right.at(_dir));
  else
  {
    if (k == K_MINUS)
      changeSpeed(-1);
    else if (k == K_PLUS)
      changeSpeed(1);
    move(_dir);
  }
}

void	snake::changeSpeed(int val)
{
  _speed += val;
  if (_speed < 2)
    _speed = 2;
  else if (_speed > 15)
    _speed = 15;
}

/*
** getters
*/

int				snake::getXmax() const
{
  return _Xmax;
}

int				snake::getYmax() const
{
  return _Ymax;
}

std::list<Position>	snake::get_snake_pos() const
{
  return _snake_pos;
}

Position			snake::get_fruit() const
{
  return _fruit;
}

std::list<Position>	snake::get_walls() const
{
  return _wall_pos;
}

int	snake::getEatenFruits() const
{
  return _eaten_fruits;
}

Direction			snake::getDirection() const
{
  return _dir;
}

int				snake::getSpeed() const
{
  return (CLOCKS_PER_SEC / _speed);
}
