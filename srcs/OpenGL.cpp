//
// super_amazing.cpp for super.c in /home/pouyet_j/test
//
// Made by Jérémy POUYET
// Login   <pouyet_j@epitech.net>
//
// Started on  Fri Mar 14 16:32:41 2014 Jérémy POUYET
// Last update Sun Apr  6 17:07:51 2014 jerome campeaux
//

#include "OpenGL.hh"

static OpenGLModule::t_color	a_colors[] =
{
  {OpenGLModule::DARK_RED, 255, 0, 0},
  {OpenGLModule::LIGHT_RED, 100, 0, 0},
  {OpenGLModule::DARK_GREEN, 0, 255, 0},
  {OpenGLModule::LIGHT_GREEN, 0, 100, 0},
  {OpenGLModule::DARK_BLUE, 0, 0, 255},
  {OpenGLModule::LIGHT_BLUE, 0, 0, 100},
  {OpenGLModule::GREY, 100, 100, 100},
  {OpenGLModule::BLACK, 0, 0, 0},
  {OpenGLModule::WHITE, 255, 255, 255},
  {OpenGLModule::NAC, -1, -1, -1}
};

extern "C" IDisplayModule	*entry_point()
{
  return (new OpenGLModule());
}

OpenGLModule::OpenGLModule() : name("lib_nibbler_OpenGL.so")
{

}

std::string const & OpenGLModule::getName() const
{
  return this->name;
}

void	OpenGLModule::init_colors()
{
  for (int i = 0; a_colors[i].name != NAC; i++)
    this->colors[a_colors[i].name] = a_colors[i];
}

int    OpenGLModule::init(const int &x, const int &y)
{
  this->x = x;
  this->y = y;
  this->init_colors();
  this->init_keys();
  if (SDL_Init(SDL_INIT_VIDEO) == -1)
    return (0);
  if (!SDL_SetVideoMode(this->x * X_SQUARE, this->y * Y_SQUARE, 32, SDL_OPENGL))
    return 0;
  SDL_WM_SetCaption("Ca c'est le nibbler de popol", NULL);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(70, (double)640/480, 1, 1000);
  gluLookAt(this->x / 2, this->y + 5, this->x, this->x / 2, this->y / 2, 0, 0, 0, 1);
  glEnable(GL_DEPTH_TEST);
  return (1);
}

void	OpenGLModule::init_keys()
{
  this->key_map[SDLK_LEFT] = game::K_RIGHT;
  this->key_map[SDLK_RIGHT] = game::K_LEFT;
  this->key_map[SDLK_p] = game::K_PAUSE;
  this->key_map[SDLK_s] = game::K_SWITCH;
  this->key_map[SDLK_KP_PLUS] = game::K_PLUS;
  this->key_map[SDLK_KP_MINUS] = game::K_MINUS;
}

void		OpenGLModule::DrawSquare(const double &x, const double &y, c_name n1, c_name n2)
{
  t_color	c1 = this->colors[n1];
  t_color	c2 = this->colors[n2];

  glBegin(GL_QUADS);

  glColor3ub(c1.red, c1.green, c1.blue);
  glVertex3d(x, y, 1);
  glVertex3d(x, y + 1, 1);
  glColor3ub(c2.red, c2.green, c2.blue);
  glVertex3d(x + 1, y + 1, 1);
  glVertex3d(x + 1, y, 1);

  glColor3ub(c1.red, c1.green, c1.blue);
  glVertex3d(x, y, 0);
  glVertex3d(x, y, 1);
  glColor3ub(c2.red, c2.green, c2.blue);
  glVertex3d(x, y + 1, 1);
  glVertex3d(x, y + 1, 0);

  glColor3ub(c1.red, c1.green, c1.blue);
  glVertex3d(x + 1, y + 1, 0);
  glVertex3d(x + 1, y, 0);
  glColor3ub(c2.red, c2.green, c2.blue);
  glVertex3d(x + 1, y, 1);
  glVertex3d(x + 1, y + 1, 1);

  glColor3ub(c1.red, c1.green, c1.blue);
  glVertex3d(x, y, 0);
  glVertex3d(x, y, 1);
  glColor3ub(c2.red, c2.green, c2.blue);
  glVertex3d(x + 1, y, 1);
  glVertex3d(x + 1, y, 0);

  glColor3ub(c1.red, c1.green, c1.blue);
  glVertex3d(x, y + 1, 0);
  glVertex3d(x, y + 1, 1);
  glColor3ub(c2.red, c2.green, c2.blue);
  glVertex3d(x + 1, y + 1, 1);
  glVertex3d(x + 1, y + 1, 0);

  glEnd();
}

void		OpenGLModule::DrawMap(const double &x, const double &y)
{
  glBegin(GL_QUADS);
  glColor3ub(100, 100, 100);
  glVertex3d(x, y, 0);
  glVertex3d(x, y + 1, 0);
  glVertex3d(x + 1, y + 1, 0);
  glVertex3d(x + 1, y, 0);
  glEnd();
}

game::keys	OpenGLModule::play()
{
  SDL_Event	event;

  while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE)
	return game::K_QUIT;
      if (event.type == SDL_KEYDOWN)
	return this->key_map[event.key.keysym.sym];
    }
  return game::NAK;
}

void		OpenGLModule::refresh(pos_list snake, pos fruit, pos_list walls, const int &eaten_fruits, const game::direction &dir)
{
  (void)dir;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for (int x = -1; x < this->x + 1; x++)
  {
    this->DrawSquare(x, -1, DARK_GREEN, DARK_RED);
    this->DrawSquare(x, this->y, DARK_GREEN, DARK_RED);
  }
  for (int y = -1; y < this->y + 1; y++)
  {
    this->DrawSquare(-1, y, DARK_GREEN, DARK_RED);
    this->DrawSquare(this->x, y, DARK_GREEN, DARK_RED);
  }
  for (int x = 0; x < this->x; x++)
    for (int y = 0; y < this->y; y++)
      this->DrawMap(x, y);
  for (std::list<game::position>::const_iterator it = snake.begin(); it != snake.end(); it++)
    {
      if (it == snake.begin())
	this->DrawSquare((*it).x, (*it).y, BLACK, WHITE);
      else
	this->DrawSquare((*it).x, (*it).y, DARK_GREEN, LIGHT_GREEN);
    }
  this->DrawSquare(fruit.x, fruit.y, DARK_RED, LIGHT_RED);
  for (std::list<game::position>::const_iterator it = walls.begin(); it != walls.end(); it++)
    this->DrawSquare((*it).x, (*it).y, DARK_BLUE, LIGHT_BLUE);
  dispScore(eaten_fruits);
  glFlush();
  SDL_GL_SwapBuffers();
}

int                   OpenGLModule::dispScore(const int &eaten_fruits)
{
  if (eaten_fruits > this->fruits_eaten)
    std::cout << "Score : " << eaten_fruits * 10 << std::endl;
  this->fruits_eaten = eaten_fruits; 
  return (0);
}

void		OpenGLModule::pause()
{
  SDL_Event	event;
  int		pause = 0;

  std::cout << "Pause, press P to continue" << std::endl;
  while (pause == 0)
    {
      SDL_WaitEvent(&event);
      if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p)
	pause = 1;
    }
}

int		OpenGLModule::end_screen()
{
  SDL_Event     event;
  game::keys    k = game::NAK;

  std::cout << "Game Over" << std::endl;
  std::cout << "Retry ? (y/n)" << std::endl;
  while (SDL_WaitEvent(&event) && k == game::NAK)
    {
      if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_n ||  event.key.keysym.sym == SDLK_ESCAPE)
	k = game::K_QUIT;
      else if (event.key.keysym.sym == SDLK_y)
	k = game::K_REPLAY;
    }
  return (k == game::K_REPLAY);
}

int	OpenGLModule::stop()
{
  SDL_Quit();
  return (1);
}

int	OpenGLModule::getX() const
{
  return this->x;
}

int	OpenGLModule::getY() const
{
  return this->y;
}
