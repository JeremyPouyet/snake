#include "DLLoader.hh"

DLLoader::DLLoader() : entry_point(NULL)
{

}

int	DLLoader::init(const std::string & name)
{
  std::string library(name);

  library.insert(0, "./");
  if (this->entry_point)
    if (dlclose(this->entry_point))
      std::cerr << "can't close the " << this->name << "library" << std::endl;
  this->name = library;
  if (!(this->entry_point = dlopen(this->name.c_str(), RTLD_LAZY)))
    {
      std::cerr << "load fail : " << this->name << std::endl;
      return (0);
    }
  return (1);
}

IDisplayModule	*DLLoader::getInstance()
{
  entry		enter;

  enter = (entry)dlsym(this->entry_point, "entry_point");
  if (!enter)
    {
      std::cerr << "can't get entry point of " << this->name << std::endl;
      return (NULL);
    }
  return enter();
}

DLLoader::~DLLoader()
{
  if (this->entry_point)
    if (dlclose(this->entry_point))
      std::cerr << "can't close the " << this->name << "library" << std::endl;
}
