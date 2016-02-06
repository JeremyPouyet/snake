#pragma once

enum		Keys
{
  NAK = 0,	K_LEFT,
  K_RIGHT,	K_QUIT,
  K_PAUSE,	K_REPLAY,
  K_SWITCH,	K_PLUS,
  K_MINUS
};

enum		Direction
{
  NAD = 0,	D_RIGHT,
  D_UP,	D_LEFT,
  D_DOWN
};

struct	Position
{
  int	x;
  int	y;
  Position(int x = 0, int y = 0);
};

struct	Move
{
  Direction	dir;
  int		x;
  int		y;
};
