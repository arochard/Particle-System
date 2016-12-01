#ifndef GRAPHIC_HPP
# define GRAPHIC_HPP

# include <vector>
# include "glew.h"
# include "glfw3.h"
# include "BaseCl.hpp"
# include "glm/glm.hpp"
# include "glm/gtc/matrix_transform.hpp"
# include "glm/gtc/type_ptr.hpp"
# include "camera.hpp"

// # define VERTICE_VBO 0
# define POSITION_VBO 0
# define COLOR_VBO 1

class Graphic
{
	private:
		GLuint			_programm_shader;
		GLuint 			_vao;
		GLFWwindow		*_win_ptr;

		void			init_window(int width, int height);
		void			create_shader();
		void 			update_fps_counter();
		void 			send_matrix(Camera *camera);

	public:
		Graphic(int width, int height);
		~Graphic();

		void			create_vbo(std::vector<GLuint> *vbos, unsigned int nbPart);
		void			draw_loop(unsigned int nbPart, BaseCl *cl, Camera *camera);
		static void		key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
	
};

#endif