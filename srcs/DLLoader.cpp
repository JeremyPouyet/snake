#include "DLLoader.hh"

DLLoader::DLLoader() :
  _entry_point(NULL)
{ }

int	DLLoader::init(const std::string & name)
{
  std::string library(name);

  library.insert(0, "./");
  if (_entry_point)
    if (dlclose(_entry_point))
      std::cerr << "can't close the " << _name << "library" << std::endl;
  _name = library;
  if (!(_entry_point = dlopen(_name.c_str(), RTLD_LAZY)))
    {
      std::cerr << "load fail : " << _name << std::endl;
      return (0);
    }
  return (1);
}

IDisplayModule	*DLLoader::getInstance()
{
  entry		enter = (entry)dlsym(_entry_point, "entry_point");

  if (!enter)
    {
      std::cerr << "can't get entry point of " << _name << std::endl;
      return (NULL);
    }
  return enter();
}

DLLoader::~DLLoader()
{
  if (_entry_point)
    if (dlclose(_entry_point))
      std::cerr << "can't close the " << _name << "library" << std::endl;
}
