#ifndef PART_SYSTEM_HPP
# define PART_SYSTEM_HPP


# include "glm/glm.hpp"
# include "Graphic.hpp"
# include "BaseCl.hpp"
# include "Camera.hpp"

# define WIDTH 1920
# define HEIGHT 1080

class PartSystem
{
	private:
		unsigned int 		_nbPart;
		std::vector<GLuint> _vbos;
		BaseCl				*_base_cl;
		Graphic				*_graphic;
		Camera				*_camera;

	public:
		PartSystem(unsigned int nbPart);
		~PartSystem();

		void				main_loop();
	
};

#endif