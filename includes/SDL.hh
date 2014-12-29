#pragma once

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <iostream>
#include <map>
#include <iterator>
#include "IDisplayModule.hh"

#define Y_SPRITE 40
#define	X_SPRITE 40

class SdlModule : public IDisplayModule
{
public:
  SdlModule();
  virtual ~SdlModule() {}
  virtual std::string const &	getName() const;
  virtual int			init(const int &x, const int &y);
  virtual int			stop();
  virtual void			refresh(pos_list snake, pos fruit, pos_list walls, const int &eaten_fruits, const game::direction &dir);
  virtual void			pause();
  virtual int			end_screen();
  virtual game::keys		play();
  virtual int			dispScore(const int &eaten_fruits);

  virtual int			getX() const;
  virtual int			getY() const;

private:
  int				init_background();
  void				dispHead(SDL_Rect &position, const game::direction &dir);
  void				init_keys();
  int				loadImage();

  std::string			name;
  int				x;
  int				y;
  SDL_Surface			*screen;
  SDL_Surface			*background;
  SDL_Surface			*grass;
  SDL_Surface			*snake;
  SDL_Surface			*sida;
  SDL_Surface			*fruit;
  SDL_Surface			*game_over;
  SDL_Surface			*head;
  std::map<int, game::keys>	key_map;

};
