#include "Game.hh"

static const game::move          aod[] =
{
  {game::D_RIGHT, 1, 0},
  {game::D_UP, 0, -1},
  {game::D_LEFT, -1, 0},
  {game::D_DOWN, 0, 1},
  {game::NAD, game::NAD, game::NAD}
};

static std::map<game::direction, game::direction> left =
{
  {game::direction::D_RIGHT, game::direction::D_UP},
  {game::direction::D_UP, game::direction::D_LEFT},
  {game::direction::D_LEFT, game::direction::D_DOWN},
  {game::direction::D_DOWN, game::direction::D_RIGHT}
};

static std::map<game::direction, game::direction> right =
{
  {game::direction::D_RIGHT, game::direction::D_DOWN},
  {game::direction::D_UP, game::direction::D_RIGHT},
  {game::direction::D_LEFT, game::direction::D_UP},
  {game::direction::D_DOWN, game::direction::D_LEFT}
};

game::position::position(int x, int y) :
  x(x), y(y)
{ }

game::snake::snake() :
  _dir(game::D_RIGHT), _eaten_fruits(0), _speed(START_SPEED)
{ }

/*
** initialization functions
*/

int			game::snake::init(const std::string &x, const std::string &y)
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
  srand(time(NULL));
  generate_fruit();
  return 1;
}

void			game::snake::generatePos()
{
  for (int i = 0; i < 4; i++)
    {
      position p((_Xmax / 2) + i - 1, _Ymax / 2);
      _snake_pos.push_front(p);
    }
}

void			game::snake::reload()
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

bool	game::snake::is_snake(int x, int y) const
{
  for (auto it = _snake_pos.begin(); it != _snake_pos.end(); it++)
    if (x == (*it).x && y == (*it).y)
      return true;
  return false;
}

bool	game::snake::is_wall(int x, int y) const
{
  for (auto it = _wall_pos.begin(); it != _wall_pos.end(); it++)
    if (x == (*it).x && y == (*it).y)
      return true;
  return false;
}

bool	game::snake::is_fruit(int x, int y) const
{
  return (x == _fruit.x && y == _fruit.y);
}

/*
** generation functions
*/

void		game::snake::generate_fruit()
{
  do
  {
    _fruit.x = rand() % _Xmax;
    _fruit.y = rand() % _Ymax;
  } while (is_snake(_fruit.x, _fruit.y) || is_wall(_fruit.x, _fruit.y));
}

void		game::snake::generate_wall()
{
  position	p;

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

bool	game::snake::finish(keys k)
{
  position p = _snake_pos.front();

  if (p.x < 0 || p.x >= _Xmax || p.y < 0 || p.y >= _Ymax)
    return true;
  _snake_pos.pop_front();
  if (is_snake(p.x, p.y))
    return true;
  _snake_pos.push_front(p);
  return (is_wall(p.x, p.y) || k == K_QUIT);
}

void	game::snake::move(game::direction d)
{
  position	p, f = _snake_pos.front();
  game::move	m;

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

void	game::snake::action(keys k)
{
  if (k == K_LEFT)
    move(left[_dir]);
  else if (k == K_RIGHT)
    move(right[_dir]);
  else if (k == NAK)
    move(_dir);
  else if (k == K_MINUS)
    changeSpeed(-1);
  else if (k == K_PLUS)
    changeSpeed(1);
}

void	game::snake::changeSpeed(int val)
{
  _speed += val;
  if (_speed < 2)
    _speed = 2;
  else if (_speed > 15)
    _speed = 15;
}

/*
** geters
*/

int				game::snake::getXmax() const
{
  return _Xmax;
}

int				game::snake::getYmax() const
{
  return _Ymax;
}

std::list<game::position>	game::snake::get_snake_pos() const
{
  return _snake_pos;
}

game::position			game::snake::get_fruit() const
{
  return _fruit;
}

std::list<game::position>	game::snake::get_walls() const
{
  return _wall_pos;
}

int	game::snake::getEatenFruits() const
{
  return _eaten_fruits;
}

game::direction			game::snake::getDirection() const
{
  return _dir;
}

int				game::snake::getSpeed() const
{
  return (CLOCKS_PER_SEC / _speed);
}
