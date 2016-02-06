#include "SDL.hh"

extern "C" IDisplayModule	*entry_point()
{
  return (new SdlModule());
}

SdlModule::SdlModule() :
  ADisplayModule("lib_nibbler_SDL.so")
{ }

bool		SdlModule::init(int x, int y)
{
  _x = x;
  _y = y;
  if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
      std::cerr << "SDL_Init fail" << std::endl;
      return false;
    }
  if (!(_screen = SDL_SetVideoMode(_x * X_SPRITE, _y * Y_SPRITE, 32, SDL_HWSURFACE)))
    {
      std::cerr << "SDL_SetVideoMode fail" << std::endl;
      return false;
    }
  SDL_WM_SetCaption("Ca c'est le nibbler de toto !", NULL);
  if (TTF_Init() == -1)
    return false;
  if (!init_background())
    {
      std::cerr << "Background initialisation fail" << std::endl;
      return false;
    }
  if (!(_snake = IMG_Load("./SDL_IMG/zgeg.png")))
    {
      std::cerr << "snake can't be load" << std::endl;
      return false;
    }
  if (!(_sida = IMG_Load("./SDL_IMG/sida.png")))
    {
      std::cerr << "sida can't be load" << std::endl;
      return false;
    }
  if (!(_fruit = IMG_Load("./SDL_IMG/viagra.png")))
    {
      std::cerr << "viagra can't be load" << std::endl;
      return false;
    }
  if (!(_game_over = IMG_Load("./SDL_IMG/game-over.jpg")))
  {
    std::cerr << "game-over can't be load" << std::endl;
    return false;
  }
  init_keys();
  return true;
}

void	SdlModule::init_keys()
{
  _key_map[SDLK_LEFT] = K_LEFT;
  _key_map[SDLK_RIGHT] = K_RIGHT;
  _key_map[SDLK_p] = K_PAUSE;
  _key_map[SDLK_s] = K_SWITCH;
  _key_map[SDLK_KP_PLUS] = K_PLUS;
  _key_map[SDLK_KP_MINUS] = K_MINUS;
}

bool	SdlModule::stop()
{
  std::cout << "stop sdl" << std::endl;
  SDL_FreeSurface(_background);
  SDL_FreeSurface(_grass);
  SDL_FreeSurface(_snake);
  SDL_FreeSurface(_sida);
  SDL_FreeSurface(_fruit);
  SDL_FreeSurface(_game_over);
  TTF_Quit();
  SDL_Quit();
  return true;
}

Keys	SdlModule::play()
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

bool		SdlModule::init_background()
{
  SDL_Rect	position;
  int		x, y;

  if (!(_background = SDL_CreateRGBSurface(SDL_HWSURFACE, _x * X_SPRITE, _y * Y_SPRITE, 32, 0, 0, 0, 0)))
    return false;
  if (!(_grass = IMG_Load("./SDL_IMG/grass.jpg")))
    return false;
  for (y = 0; y < _y; y++)
    {
      position.y = y * X_SPRITE;
      for (x = 0; x < _x; x++)
	{
	  position.x = x * X_SPRITE;
	  SDL_BlitSurface(_grass, NULL, _background, &position);
	}
    }
  return true;
}

void		SdlModule::refresh(const pos_list &snake, const pos &fruit, const pos_list &walls, int eaten_fruits, Direction dir)
{
  SDL_Rect	position;

  position.x = 0;
  position.y = 0;
  SDL_BlitSurface(_background, NULL, _screen, &position);
  position.x = fruit.x * X_SPRITE;
  position.y = fruit.y * Y_SPRITE;
  SDL_BlitSurface(_fruit, NULL, _screen, &position);
  for (auto it = snake.begin(); it != snake.end(); it++)
    {
      position.x = (*it).x * X_SPRITE;
      position.y = (*it).y * Y_SPRITE;
      if (it == snake.begin())
	dispHead(position, dir);
      else
	SDL_BlitSurface(_snake, NULL, _screen, &position);
    }
  for (auto i : walls)
    {
      position.x = i.x * X_SPRITE;
      position.y = i.y * Y_SPRITE;
      SDL_BlitSurface(_sida, NULL, _screen, &position);
    }
  if (dispScore(eaten_fruits) == -1)
    return ;
  SDL_Flip(_screen);
}

void	SdlModule::dispHead(SDL_Rect &position, Direction dir)
{
  std::string	load;
  const std::map<Direction, std::string>	display = {
    {D_RIGHT, "./SDL_IMG/head_right.png"},
    {D_LEFT, "./SDL_IMG/head_left.png"},
    {D_UP, "./SDL_IMG/head_up.png"},
    {D_DOWN, "./SDL_IMG/head_down.png"}
  };
  for (auto it : display)
    if (it.first == dir)
      load = it.second;
  if (!(_head = IMG_Load(load.c_str())))
    {
      std::cerr << "head can't be load" << std::endl;
      return ;
    }
  SDL_BlitSurface(_head, NULL, _screen, &position);
}

int		SdlModule::dispScore(int eaten_fruits)
{
  TTF_Font	*write = NULL;
  char		score[20];
  SDL_Surface	*txt = NULL;
  SDL_Color	couleurNoire = {0, 0, 0, 0};
  SDL_Rect	position;

  if ((write = TTF_OpenFont("./ttf/game_over.ttf", 60)) == NULL)
    return (-1);
  sprintf(score, "Score : %d", eaten_fruits * 10);
  if (!(txt = TTF_RenderText_Blended(write, score, couleurNoire)))
    return (-1);
  position.x = 0;
  position.y = 0;
  if (SDL_BlitSurface(txt, NULL, _screen, &position) == -1)
    return (-1);
  SDL_FreeSurface(txt);
  TTF_CloseFont(write);
  return (0);
}

void	SdlModule::pause()
{
  SDL_Event	event;
  TTF_Font	*write = NULL;
  SDL_Surface	*txt = NULL;
  SDL_Color	couleurNoire = {0, 0, 0, 0};
  SDL_Rect	position;

  if (TTF_Init() == -1)
    return ;
  if ((write = TTF_OpenFont("./ttf/game_over.ttf", 100)) == NULL)
    return ;
  if (!(txt = TTF_RenderText_Blended(write, "Pause", couleurNoire)))
    return ;
  position.x = ((_x * X_SPRITE) / 2) - (2 * X_SPRITE);
  position.y = ((_y * Y_SPRITE) / 2) - (2 * Y_SPRITE);
  if (SDL_BlitSurface(txt, NULL, _screen, &position) == -1)
    return ;
  if (!(txt = TTF_RenderText_Blended(write, " Press P to continue", couleurNoire)))
    return ;
  position.x = ((_x * X_SPRITE) / 2) - (5 * X_SPRITE);
  position.y = ((_y * Y_SPRITE) / 2) - (1 * Y_SPRITE);
  if (SDL_BlitSurface(txt, NULL, _screen, &position) == -1)
    return ;
  SDL_Flip(_screen);
  SDL_FreeSurface(txt);
  TTF_CloseFont(write);
  TTF_Quit();
  do
  {
    SDL_WaitEvent(&event);
  } while (!(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p));
}

int	SdlModule::end_screen()
{
  SDL_Event	event;
  Keys		k = NAK;
  SDL_Rect	position;

  position.x = ((_x * X_SPRITE) / 2) - (2 * X_SPRITE);
  position.y = ((_y * Y_SPRITE) / 2) - (2 * Y_SPRITE);
  SDL_BlitSurface(_game_over, NULL, _screen, &position);
  SDL_Flip(_screen);
  while (k == NAK)
  {
    SDL_WaitEvent(&event);
    if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_n || event.key.keysym.sym == SDLK_ESCAPE)
      k = K_QUIT;
    else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_y)
      k = K_REPLAY;
  }
  return (k == K_REPLAY);
}
