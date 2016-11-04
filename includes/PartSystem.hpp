#ifndef PART_SYSTEM_HPP
# define PART_SYSTEM_HPP

# include "Graphic.hpp"
# include "BaseCl.hpp"

# define WIDTH 1920
# define HEIGHT 1080

class PartSystem
{
	private:
		unsigned int 		_nbPart;
		BaseCl				*_base_cl;
		Graphic				*_graphic;

	public:
		PartSystem(unsigned int nbPart);
		~PartSystem();
	
};

#endif