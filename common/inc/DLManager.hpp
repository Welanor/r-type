#ifndef DLMANAGER_HPP_
# define DLMANAGER_HPP_

# include	<map>

# include	"DLLoaderUnix.hpp"
# include	"DLLoaderWin.hpp"
# include	"DirHandlerUnix.hpp"
# include	"DirHandlerWin.hpp"

template<typename T>											//T: IMonsters
class DLManager
{
private:
	std::map<std::string, DLLoader<T>> modules;
	DirHandler	*dirhandler;

	bool LoadFromRep(const std::string &rep)					//ret: past the path and name of the repertory where are the monster 's libs : "../Debug"
	{
		std::list<std::string>	list;

		list = this->dirhandler->GetLibsFromDir(rep);
		if (list.empty())
		{
			std::cout << "List empty" << std::endl;
			return false;
		}
		else
			std::cout << "list not empty" << std::endl;
		for (std::list<std::string>::iterator it = list.begin(); it != list.end(); ++it)
		{
			std::string name = (*it);
			DLLoader<T>	load = DLLoader<T>(name);
			this->modules.insert(std::pair<std::string, DLLoader<T>>(name, load));
		}
		return true;
	}

public:
	DLManager(const std::string &rep)
	{
		this->dirhandler = dirhandler->GetInstance();
		this->LoadFromRep(rep);
	}

	~DLManager()
	{
		if (!this->modules.empty())
			this->modules.clear();
	}

	const T	&GetInstance(const std::string &name)				//name: is the name of the lib monster
	{
		return this->modules[name].GetInstance();
	}

protected:
};

#endif // !DLMANAGER_HPP_
