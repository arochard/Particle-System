#ifndef PART_SYSTEM_HPP
# define PART_SYSTEM_HPP



# include "glew.h"
# include "glfw3.h"
# include "BaseCl.hpp"

class PartSystem
{
	private:
		unsigned int 		_nbPart;
		BaseCl				*_base_cl;

	public:
		PartSystem(unsigned int nbPart);
		~PartSystem();
	
};

#endif