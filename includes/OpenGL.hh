//
// super_amazing.hh for tp in /home/pouyet_j/test
//
// Made by Jérémy POUYET
// Login   <pouyet_j@epitech.net>
//
// Started on  Sat Mar 15 15:08:53 2014 Jérémy POUYET
// Last update Sun Apr  6 17:06:36 2014 jerome campeaux
//

#ifndef OPENGL_HH_
# define OPENGL_HH_

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
  virtual ~OpenGLModule() {}
  virtual std::string const &	getName() const;
  virtual int			init(const int &x, const int &y);
  virtual game::keys		play();
  virtual void			refresh(pos_list snake, pos fruit, pos_list walls, const int &eaten_fruits, const game::direction &dir);
  virtual void			pause();
  virtual int                   dispScore(const int &eaten_fruits);
  virtual int			end_screen();
  virtual int			stop();
  virtual int			getX() const;
  virtual int			getY() const;
private:

  int x;
  int y;
  std::string			name;
  SDL_Surface			*game_over;
  SDL_Surface			*screen;
  std::map<c_name, t_color>	colors;
  std::map<int, game::keys>	key_map;
  int				fruits_eaten;

  void	init_colors();
  void	init_keys();
  void	DrawSquare(const double &width, const double &height, c_name n1, c_name n2);
  void	DrawMap(const double &width, const double &height);

};

#endif
