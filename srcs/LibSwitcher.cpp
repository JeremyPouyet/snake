#include "LibSwitcher.hh"

LibSwitcher::LibSwitcher()
{
  this->init_libs();
}

LibSwitcher::~LibSwitcher()
{

}

void	LibSwitcher::init_libs()
{
  this->libs.push_front("lib_nibbler_Ncurses.so");
  this->libs.push_front("lib_nibbler_SDL.so");
  this->libs.push_front("lib_nibbler_OpenGL.so");
}

IDisplayModule	*LibSwitcher::load(const std::string &name)
{
  if (!this->loader.init(name))
    return (NULL);
  return this->loader.getInstance();
}

IDisplayModule		*LibSwitcher::change(IDisplayModule *mod)
{
  IDisplayModule	*m;
  int x = mod->getX();
  int y = mod->getY();

  this->libs.push_back(this->libs.front());
  this->libs.pop_front();
  if (mod->getName() == this->libs.front())
  {
    this->libs.push_back(this->libs.front());
    this->libs.pop_front();
  }
  this->stop(mod);
  this->loader.init(this->libs.front());
  if (!(m = this->loader.getInstance()))
    return (NULL);
  if (!m->init(x, y))
    return (NULL);
  return m;
}

void	LibSwitcher::stop(IDisplayModule *mod)
{
  mod->stop();
  delete mod;
}
