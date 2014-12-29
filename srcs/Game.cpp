#include "Game.hh"

static game::move          aod[] =
  {
    {game::D_RIGHT, 1, 0},
    {game::D_UP, 0, -1},
    {game::D_LEFT, -1, 0},
    {game::D_DOWN, 0, 1},
    {game::NAD, game::NAD, game::NAD}
  };

game::position::position(int x, int y) : x(x), y(y)
{

}

game::snake::snake()
  : dir(game::D_RIGHT), eaten_fruits(0), speed(START_SPEED)
{

}

/*
** initialization functions
*/

void			game::snake::init_left()
{
  this->left[D_RIGHT]	= D_UP;
  this->left[D_UP]	= D_LEFT;
  this->left[D_LEFT]	= D_DOWN;
  this->left[D_DOWN]	= D_RIGHT;
}

void			game::snake::init_right()
{
  this->right[D_RIGHT]	= D_DOWN;
  this->right[D_UP]	= D_RIGHT;
  this->right[D_LEFT]	= D_UP;
  this->right[D_DOWN]	= D_LEFT;
}

int			game::snake::init(const std::string &x, const std::string &y)
{
  std::istringstream	convert;

  convert.str(x);
  convert >> this->Xmax;
  convert.clear();
  convert.str(y);
  convert >> this->Ymax;
  if (this->Xmax > MAX_X || this->Ymax > MAX_Y)
    {
      std::cerr << "Asked map is too big (Max : 48 * 26)" << std::endl;
      return 0;
    }
  if (this->Xmax < MIN_Y || this->Ymax < MIN_Y)
    {
      std::cerr << "Asked map is too short (Min : 15 * 15)" << std::endl;
      return 0;
    }
  this->generatePos();
  srand(time(NULL));
  this->generate_fruit();
  this->init_left();
  this->init_right();
  return 1;
}

void			game::snake::generatePos()
{
  for (unsigned int i = 0; i < 4; i++)
    {
      position p((this->Xmax / 2) + i - 1, this->Ymax / 2);
      this->snake_pos.push_front(p);
    }
}

void			game::snake::reload()
{
  this->snake_pos.clear();
  this->wall_pos.clear();
  this->eaten_fruits = 0;
  this->speed = START_SPEED;
  this->dir = D_RIGHT;
  this->generatePos();
  this->generate_fruit();
}

/*
** check functions
*/

int	game::snake::is_snake(const int &x, const int &y) const
{
  std::list<game::position>::const_iterator	it;

  for (it = snake_pos.begin(); it != snake_pos.end(); it++)
    if (x == (*it).x && y == (*it).y)
      return 1;
  return 0;
}

int	game::snake::is_wall(const int &x, const int &y) const
{
  std::list<game::position>::const_iterator	it;

  for (it = wall_pos.begin(); it != wall_pos.end(); it++)
    if (x == (*it).x && y == (*it).y)
      return 1;
  return 0;
}

int	game::snake::is_fruit(const int &x, const int &y) const
{
  return (x == this->fruit.x && y == this->fruit.y);
}

/*
** generation functions
*/


void		game::snake::generate_fruit()
{
  int		not_find = 1;

  while (not_find)
    {
      this->fruit.x = rand() % this->Xmax;
      this->fruit.y = rand() % this->Ymax;
      not_find = (this->is_snake(this->fruit.x, this->fruit.y) ||
		  this->is_wall(this->fruit.x, this->fruit.y));
    }
}

void		game::snake::generate_wall()
{
  int		not_find = 1;
  position	p;

  while (not_find)
    {
      p.x = rand() % this->Xmax;
      p.y = rand() % this->Ymax;
      not_find = (this->is_snake(p.x, p.y) ||
		  this->is_fruit(p.x, p.y));
    }
  this->wall_pos.push_back(p);
}

/*
** game logic functions
*/

int	game::snake::finish(const keys &k)
{
  position p = this->snake_pos.front();
  std::list<game::position>::const_iterator	it;

  if (p.x < 0 || p.x >= this->Xmax || p.y < 0 || p.y >= this->Ymax)
    return 1;
  this->snake_pos.pop_front();
  if (this->is_snake(p.x, p.y))
    return 1;
  this->snake_pos.push_front(p);
  return (this->is_wall(p.x, p.y) || k == K_QUIT);
}

void	game::snake::move(const game::direction &d)
{
  position	f = this->snake_pos.front();
  position	p;
  game::move	m;

  for (unsigned int i = 0; aod[i].dir != NAD; i++)
    if (aod[i].dir == d)
      {
	this->dir = aod[i].dir;
	m = aod[i];
      }
  p.x = f.x += m.x;
  p.y = f.y += m.y;
  this->snake_pos.push_front(p);
  if (this->is_fruit(p.x, p.y))
    {
      this->generate_fruit();
      if (++this->eaten_fruits % 3 == 0)
	this->generate_wall();
    }
  else
    this->snake_pos.pop_back();
}

void	game::snake::action(const keys &k)
{
  if (k == K_LEFT)
    this->move(this->left[this->dir]);
  else if (k == K_RIGHT)
    this->move(this->right[this->dir]);
  else if (k == NAK)
    this->move(this->dir);
  else if (k == K_MINUS)
    this->changeSpeed(-1);
  else if (k == K_PLUS)
    this->changeSpeed(1);
}

void	game::snake::changeSpeed(int val)
{
  this->speed += val;
  if (this->speed < 2)
    this->speed = 2;
  else if (this->speed > 15)
    this->speed = 15;
}

/*
** geters
*/

int				game::snake::getXmax() const
{
  return this->Xmax;
}

int				game::snake::getYmax() const
{
  return this->Ymax;
}

std::list<game::position>	game::snake::get_snake_pos() const
{
  return this->snake_pos;
}

game::position			game::snake::get_fruit() const
{
  return this->fruit;
}

std::list<game::position>	game::snake::get_walls() const
{
  return this->wall_pos;
}

const int			&game::snake::getEatenFruits() const
{
  return this->eaten_fruits;
}

game::direction			game::snake::getDirection() const
{
  return this->dir;
}

int				game::snake::getSpeed() const
{
  return (CLOCKS_PER_SEC / this->speed);
}
