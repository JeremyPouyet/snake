#include "SDL.hh"

extern "C" IDisplayModule	*entry_point()
{
  return (new SdlModule());
}

SdlModule::SdlModule() : name("lib_nibbler_SDL.so")
{

}

std::string const & SdlModule::getName() const
{
  return this->name;
}

int		SdlModule::init(const int &x, const int &y)
{
  this->x = x;
  this->y = y;
  if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
      std::cerr << "SDL_Init fail" << std::endl;
      return (0);
    }
  if (!(this->screen = SDL_SetVideoMode(this->x * X_SPRITE, this->y * Y_SPRITE, 32, SDL_HWSURFACE)))
    {
      std::cerr << "SDL_SetVideoMode fail" << std::endl;
      return 0;
    }
  SDL_WM_SetCaption("Ca c'est le nibbler de toto !", NULL);
  if (TTF_Init() == -1)
    return (-1);
  if (!this->init_background())
    {
      std::cerr << "Background initialisation fail" << std::endl;
      return 0;
    }
  if (!(this->snake = IMG_Load("SDL_IMG/zgeg.png")))
    {
      std::cerr << "snake can't be load" << std::endl;
      return 0;
    }
  if (!(this->sida = IMG_Load("SDL_IMG/sida.png")))
    {
      std::cerr << "sida can't be load" << std::endl;
      return 0;
    }
  if (!(this->fruit = IMG_Load("SDL_IMG/viagra.png")))
    {
      std::cerr << "viagra can't be load" << std::endl;
      return 0;
    }
  if (!(this->game_over = IMG_Load("SDL_IMG/game-over.jpg")))
  {
    std::cerr << "game-over can't be load" << std::endl;
    return 0;
  }
  this->init_keys();
  return (1);
}

void	SdlModule::init_keys()
{
  this->key_map[SDLK_LEFT] = game::K_LEFT;
  this->key_map[SDLK_RIGHT] = game::K_RIGHT;
  this->key_map[SDLK_p] = game::K_PAUSE;
  this->key_map[SDLK_s] = game::K_SWITCH;
  this->key_map[SDLK_KP_PLUS] = game::K_PLUS;
  this->key_map[SDLK_KP_MINUS] = game::K_MINUS;
}

int	SdlModule::stop()
{
  SDL_FreeSurface(this->background);
  SDL_FreeSurface(this->grass);
  SDL_FreeSurface(this->snake);
  SDL_FreeSurface(this->sida);
  SDL_FreeSurface(this->fruit);
  SDL_FreeSurface(this->game_over);
  TTF_Quit();
  SDL_Quit();
  return (1);
}

game::keys	SdlModule::play()
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

int		SdlModule::init_background()
{
  SDL_Rect	position;
  int		x, y;

  if (!(this->background = SDL_CreateRGBSurface(SDL_HWSURFACE, this->x * X_SPRITE, this->y * Y_SPRITE, 32, 0, 0, 0, 0)))
    return 0;
  if (!(this->grass = IMG_Load("SDL_IMG/grass.jpg")))
    return 0;
  for (y = 0; y < this->y; y++)
    {
      position.y = y * X_SPRITE;
      for (x = 0; x < this->x; x++)
	{
	  position.x = x * X_SPRITE;
	  SDL_BlitSurface(this->grass, NULL, this->background, &position);
	}
    }
  return 1;
}

void		SdlModule::refresh(pos_list snake, pos fruit, pos_list walls, const int &eaten_fruits, const game::direction &dir)
{
  SDL_Rect	position;

  position.x = 0;
  position.y = 0;
  SDL_BlitSurface(this->background, NULL, this->screen, &position);
  position.x = fruit.x * X_SPRITE;
  position.y = fruit.y * Y_SPRITE;
  SDL_BlitSurface(this->fruit, NULL, this->screen, &position);
  for (std::list<game::position>::const_iterator it = snake.begin(); it != snake.end(); it++)
    {
      position.x = (*it).x * X_SPRITE;
      position.y = (*it).y * Y_SPRITE;
      if (it == snake.begin())
	dispHead(position, dir);
      else
	SDL_BlitSurface(this->snake, NULL, this->screen, &position);
    }
  for (std::list<game::position>::const_iterator i = walls.begin(); i != walls.end(); i++)
    {
      position.x = (*i).x * X_SPRITE;
      position.y = (*i).y * Y_SPRITE;
      SDL_BlitSurface(this->sida, NULL, this->screen, &position);
    }
  if (dispScore(eaten_fruits) == -1)
    return ;
  SDL_Flip(this->screen);
}

void	SdlModule::dispHead(SDL_Rect &position, const game::direction &dir)
{
  std::string	load;

  std::map<game::direction, std::string>	display;
  display[game::D_RIGHT] = "SDL_IMG/head_right.png";
  display[game::D_LEFT] = "SDL_IMG/head_left.png";
  display[game::D_UP] = "SDL_IMG/head_up.png";
  display[game::D_DOWN] = "SDL_IMG/head_down.png";
  for (std::map<game::direction, std::string>::iterator it=display.begin(); it != display.end(); ++it)
    if (it->first == dir)
      load = it->second;
  if (!(this->head = IMG_Load(load.c_str())))
    {
      std::cerr << "head can't be load" << std::endl;
      return ;
    }
  SDL_BlitSurface(this->head, NULL, this->screen, &position);
}

int		SdlModule::dispScore(const int &eaten_fruits)
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
  if (SDL_BlitSurface(txt, NULL, this->screen, &position) == -1)
    return (-1);
  SDL_FreeSurface(txt);
  TTF_CloseFont(write);
  return (0);
}

void	SdlModule::pause()
{
  SDL_Event	event;
  int		pause = 0;
  TTF_Font	*write = NULL;
  SDL_Surface	*txt = NULL;
  SDL_Color	couleurNoire = {0, 0, 0, 0};
  SDL_Rect	position;

  if (TTF_Init() == -1)
    return ;
  if ((write = TTF_OpenFont("./ttf/game_over.ttf", 100)) == NULL)
    return
;  if (!(txt = TTF_RenderText_Blended(write, "Pause", couleurNoire)))
    return ;
  position.x = ((this->x * X_SPRITE) / 2) - (2 * X_SPRITE);
  position.y = ((this->y * Y_SPRITE) / 2) - (2 * Y_SPRITE);
  if (SDL_BlitSurface(txt, NULL, this->screen, &position) == -1)
    return ;
  if (!(txt = TTF_RenderText_Blended(write, " Press P to continue", couleurNoire)))
    return ;
  position.x = ((this->x * X_SPRITE) / 2) - (5 * X_SPRITE);
  position.y = ((this->y * Y_SPRITE) / 2) - (1 * Y_SPRITE);
  if (SDL_BlitSurface(txt, NULL, this->screen, &position) == -1)
    return ;
  SDL_Flip(this->screen);
  SDL_FreeSurface(txt);
  TTF_CloseFont(write);
  TTF_Quit();
  while (pause == 0)
    {
      SDL_WaitEvent(&event);
      if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p)
	  pause = 1;
    }
}

int	SdlModule::end_screen()
{
  SDL_Event	event;
  game::keys	k = game::NAK;
  SDL_Rect	position;

  position.x = ((this->x * X_SPRITE) / 2) - (2 * X_SPRITE);
  position.y = ((this->y * Y_SPRITE) / 2) - (2 * Y_SPRITE);
  SDL_BlitSurface(this->game_over, NULL, this->screen, &position);
  SDL_Flip(this->screen);
  while (k == game::NAK)
  {
    SDL_WaitEvent(&event);
    if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_n || event.key.keysym.sym == SDLK_ESCAPE)
      k = game::K_QUIT;
    else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_y)
      k = game::K_REPLAY;
  }
  return (k == game::K_REPLAY);
}

int	SdlModule::getX() const
{
  return this->x;
}

int	SdlModule::getY() const
{
  return this->y;
}
