#ifndef AFSHANDLER_HH_
# define AFSHANDLER_HH_

# include <regex>
# include <string>
# include <list>
# include <exception>
# include <stdexcept>

# include "Singleton.hpp"

namespace FileSystem
{

class AFSHandler : public Singleton<AFSHandler>
{
  friend class Singleton<AFSHandler>;

public:
  AFSHandler() = default;
  virtual ~AFSHandler() = default;

  virtual std::string filePath(const std::string& path) const
  {
    size_t found = path.find_last_of("/\\");
    if (found == std::string::npos)
      return "";
    return path.substr(0, found);
  };

  virtual std::string fileName(const std::string& path) const
  {
    size_t found = path.find_last_of("/\\");
    if (found == std::string::npos)
      return path;
    if (found + 1 >= path.length())
      return "";
    return path.substr(found + 1);
  };

  virtual std::string fileExtension(const std::string& path) const
  {
    std::string file = fileName(path);
    size_t found = path.find_last_of(".");
    if (found == std::string::npos)
      return "";
    return file.substr(found);
  };

  virtual std::string currentPath() const = 0;
  virtual std::list<std::string> listDir(const std::string& dir) const = 0;
};


class FileSystemError : public std::runtime_error
{
public:
  FileSystemError(const std::string& w) throw() : runtime_error(w) {};
  virtual ~FileSystemError() throw() {};
};

};

#endif // !AFSHANDLER_HH_
