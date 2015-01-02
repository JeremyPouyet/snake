#pragma once

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <map>
#include "IDisplayModule.hh"

#define X_SQUARE 40
#define Y_SQUARE 40

class OpenGLModule : public IDisplayModule
{
public:

  typedef enum	c_name
  {
    DARK_RED, LIGHT_RED,
    DARK_GREEN, LIGHT_GREEN,
    DARK_BLUE, LIGHT_BLUE,
    BLACK, WHITE,
    GREY,
    NAC
  }		c_name;

  typedef struct	s_color
  {
    c_name		name;
    int			red;
    int			green;
    int			blue;
  }			t_color;

  OpenGLModule();
  ~OpenGLModule() {}
  std::string const &	getName() const;
  bool			init(int x, int y);
  game::keys		play();
  void			refresh(const pos_list &snake, const pos &fruit, const pos_list &walls, int eaten_fruits, game::direction dir);
  void			pause();
  int                   dispScore(int eaten_fruits);
  int			end_screen();
  bool			stop();
  int			getX() const;
  int			getY() const;
private:

  int _x;
  int _y;
  std::string			_name;
  SDL_Surface			*_game_over;
  SDL_Surface			*_screen;
  std::map<c_name, t_color>	_colors;
  std::map<int, game::keys>	_key_map;
  int				_fruits_eaten;

  void	init_colors();
  void	init_keys();
  void	DrawSquare(double width, double height, c_name n1, c_name n2);
  void	DrawMap(double width, double height);

};
