#include "OpenGL.hh"

static const OpenGLModule::t_color	a_colors[] =
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

OpenGLModule::OpenGLModule() :
  ADisplayModule("lib_nibbler_OpenGL.so")
{ }

void	OpenGLModule::init_colors()
{
  for (int i = 0; a_colors[i].name != NAC; i++)
    _colors[a_colors[i].name] = a_colors[i];
}

bool    OpenGLModule::init(int x, int y)
{
  if (SDL_Init(SDL_INIT_VIDEO) == -1)
    return false;
  _x = x;
  _y = y;
  init_colors();
  init_keys();
  if (!SDL_SetVideoMode(_x * X_SQUARE, _y * Y_SQUARE, 32, SDL_OPENGL))
    return false;
  SDL_WM_SetCaption("Ca c'est le nibbler de popol", NULL);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(70, (double)640/480, 1, 1000);
  gluLookAt(_x / 2, _y + 5, _x, _x / 2, _y / 2, 0, 0, 0, 1);
  glEnable(GL_DEPTH_TEST);
  return true;
}

void	OpenGLModule::init_keys()
{
  _key_map[SDLK_LEFT] = K_RIGHT;
  _key_map[SDLK_RIGHT] = K_LEFT;
  _key_map[SDLK_p] = K_PAUSE;
  _key_map[SDLK_s] = K_SWITCH;
  _key_map[SDLK_KP_PLUS] = K_PLUS;
  _key_map[SDLK_KP_MINUS] = K_MINUS;
}

void		OpenGLModule::DrawSquare(double x, double y, c_name n1, c_name n2)
{
  t_color	c1 = _colors[n1];
  t_color	c2 = _colors[n2];

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

void		OpenGLModule::DrawMap(double x, double y)
{
  glBegin(GL_QUADS);
  glColor3ub(100, 100, 100);
  glVertex3d(x, y, 0);
  glVertex3d(x, y + 1, 0);
  glVertex3d(x + 1, y + 1, 0);
  glVertex3d(x + 1, y, 0);
  glEnd();
}

Keys	OpenGLModule::play()
{
  SDL_Event	event;

  while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE)
	return K_QUIT;
      if (event.type == SDL_KEYDOWN)
	return _key_map[event.key.keysym.sym];
    }
  return NAK;
}

void		OpenGLModule::refresh(const pos_list &snake, const pos &fruit, const pos_list &walls, int eaten_fruits, Direction dir)
{
  (void)dir;
  int x, y;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for (x = -1; x < _x + 1; x++)
  {
    DrawSquare(x, -1, DARK_GREEN, DARK_RED);
    DrawSquare(x, _y, DARK_GREEN, DARK_RED);
  }
  for (y = -1; y < _y + 1; y++)
  {
    DrawSquare(-1, y, DARK_GREEN, DARK_RED);
    DrawSquare(_x, y, DARK_GREEN, DARK_RED);
  }
  for (x = 0; x < _x; x++)
    for (y = 0; y < _y; y++)
      DrawMap(x, y);
  for (auto it = snake.begin(); it != snake.end(); it++)
    {
      if (it == snake.begin())
	DrawSquare((*it).x, (*it).y, BLACK, WHITE);
      else
	DrawSquare((*it).x, (*it).y, DARK_GREEN, LIGHT_GREEN);
    }
  DrawSquare(fruit.x, fruit.y, DARK_RED, LIGHT_RED);
  for (auto it = walls.begin(); it != walls.end(); it++)
    DrawSquare((*it).x, (*it).y, DARK_BLUE, LIGHT_BLUE);
  dispScore(eaten_fruits);
  glFlush();
  SDL_GL_SwapBuffers();
}

int             OpenGLModule::dispScore(int eaten_fruits)
{
  if (eaten_fruits > _fruits_eaten)
    std::cout << "Score : " << eaten_fruits * 10 << std::endl;
  _fruits_eaten = eaten_fruits;
  return (0);
}

void		OpenGLModule::pause()
{
  SDL_Event	event;

  std::cout << "Pause, press P to continue" << std::endl;
  do
  {
    SDL_WaitEvent(&event);
  } while (!(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p));
}

int		OpenGLModule::end_screen()
{
  SDL_Event     event;
  Keys		k = NAK;

  std::cout << "Game Over" << std::endl
	    << "Retry ? (y/n)" << std::endl;
  while (SDL_WaitEvent(&event) && k == NAK)
    {
      if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_n ||  event.key.keysym.sym == SDLK_ESCAPE)
	k = K_QUIT;
      else if (event.key.keysym.sym == SDLK_y)
	k = K_REPLAY;
    }
  return (k == K_REPLAY);
}

bool	OpenGLModule::stop()
{
  SDL_Quit();
  return true;
}
