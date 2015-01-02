#include "LibSwitcher.hh"

LibSwitcher::LibSwitcher()
{
  this->init_libs();
}

void	LibSwitcher::init_libs()
{
  _libs.push_front("./libs/lib_nibbler_Ncurses.so");
  _libs.push_front("./libs/lib_nibbler_SDL.so");
  _libs.push_front("./libs/lib_nibbler_OpenGL.so");
}

IDisplayModule	*LibSwitcher::load(const std::string &name)
{
  if (!_loader.init(name))
    return (NULL);
  return _loader.getInstance();
}

IDisplayModule		*LibSwitcher::change(IDisplayModule *mod)
{
  IDisplayModule	*m;

  _libs.push_back(_libs.front());
  _libs.pop_front();
  if (mod->getName() == _libs.front())
  {
    _libs.push_back(_libs.front());
    _libs.pop_front();
  }
  int x = mod->getX(), y = mod->getY();
  stop(mod);
  _loader.init(_libs.front());
  if (!(m = _loader.getInstance()))
    return NULL;
  if (!m->init(x, y))
    return NULL;
  return m;
}

void	LibSwitcher::stop(IDisplayModule *mod)
{
  mod->stop();
  delete mod;
}
