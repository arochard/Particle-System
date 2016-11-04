#ifndef GRAPHIC_HPP
# define GRAPHIC_HPP

# include <iostream>
# include <fstream>
# include "glew.h"
# include "glfw3.h"
# include "Exception.hpp"

class Graphic
{
	private:
		GLuint		_programm_shader;
		GLFWwindow	*_win_ptr;

		void		init_window(int width, int height);
		void		create_shader();
		void 		update_fps_counter();
	public:
		Graphic(int width, int height);
		~Graphic();

		void 		main_loop();
	
};

#endif