#pragma once

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <iostream>
#include <map>
#include <iterator>
#include "DisplayModule.hh"

#define Y_SPRITE 40
#define	X_SPRITE 40

class SdlModule : public ADisplayModule
{
public:
  SdlModule();
  ~SdlModule() {}
  bool	init(int x, int y);
  bool	stop();
  void	refresh(const pos_list &snake, const pos &fruit, const pos_list &walls, int eaten_fruits, Direction dir);
  void	pause();
  int	end_screen();
  Keys	play();
  int	dispScore(int eaten_fruits);

private:
  bool	init_background();
  void	dispHead(SDL_Rect &position, Direction dir);
  void	init_keys();
  int	loadImage();

  std::string		_name;
  int			_x;
  int			_y;
  SDL_Surface		*_screen;
  SDL_Surface		*_background;
  SDL_Surface		*_grass;
  SDL_Surface		*_snake;
  SDL_Surface		*_sida;
  SDL_Surface		*_fruit;
  SDL_Surface		*_game_over;
  SDL_Surface		*_head;
  std::map<int, Keys>	_key_map;

};
