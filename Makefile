##
## Makefile for tp in /home/pouyet_j/test
##
## Made by Jérémy POUYET
## Login   <pouyet_j@epitech.net>
##
## Started on  Fri Mar 14 14:06:41 2014 Jérémy POUYET
## Last update Sun Apr  6 14:42:56 2014 Jérémy POUYET
##

##
## Lib an program name
##
NAME		= nibbler

LIB1		= lib_nibbler_SDL.so
LIB2		= lib_nibbler_Ncurses.so
LIB3		= lib_nibbler_OpenGL.so

##
## sources
##
NSRCS		= srcs/main.cpp \
		srcs/DLLoader.cpp \
		srcs/Game.cpp \
		srcs/LibSwitcher.cpp

SRC1		= srcs/SDL.cpp
SRC2		= srcs/Ncurses.cpp
SRC3		= srcs/OpenGL.cpp

##
## object maker
##
NOBJS		= $(NSRCS:.cpp=.o)

OBJ1		= $(SRC1:.cpp=.o)
OBJ2		= $(SRC2:.cpp=.o)
OBJ3		= $(SRC3:.cpp=.o)

##
## compilation falgs
##
CC		= g++

CPPFLAGS	+= -std=c++11
CPPFLAGS	+= -W -Wall -Werror
CPPFLAGS	+= -fPIC
CPPFLAGS	+= -I./includes

LDFLAGS		= -shared

LSDL		= -lSDL -lSDL_image -lSDL_ttf
LNCURSES	= -lncurses
LOPENGL		= -lGL -lGLU -lSDL
LIBDL		= -ldl

EXTERN_LIBS	= $(LSDL) $(LNCURSES) $(LOPENGL)

RM		= rm -rf

all:		$(NAME)

##
## compilation
##
$(NAME):	$(OBJ1) $(OBJ2) $(OBJ3) $(NOBJS)
		$(CC) $(LDFLAGS) $(OBJ1) -o libs/$(LIB1) $(LSDL)
		$(CC) $(LDFLAGS) $(OBJ2) -o libs/$(LIB2) $(LNCURSES)
		$(CC) $(LDFLAGS) $(OBJ3) -o libs/$(LIB3) $(LOPENGL)
		$(CC) $(NOBJS) -o $(NAME) $(LIBDL)

##
## clean
##
clean:
		$(RM) $(OBJ1)
		$(RM) $(OBJ2)
		$(RM) $(OBJ3)
		$(RM) $(NOBJS)

fclean:		clean
		$(RM) $(NAME)
		$(RM) $(LIB1)
		$(RM) $(LIB2)
		$(RM) $(LIB3)

re:		fclean all

.PHONY:		all clean fclean re
