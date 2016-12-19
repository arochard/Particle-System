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

# define SPHERE 1
# define CUBE 2

class Graphic
{
	private:
		unsigned int 	_begin_form;
		GLuint			_programm_shader;
		GLuint 			_vao;
		GLFWwindow		*_win_ptr;
		Camera 			*_camera;
		int 			_width;
		int 			_height;
		static Camera	*_camera_ptr;
		static float 	_deltaTime;
		static bool 	_button_pressed;

		void			init_window(int width, int height);
		void			create_shader();
		void 			update_fps_counter();
		void 			send_matrix();

	public:
		Graphic(int width, int height);
		~Graphic();

		void			create_vbo(std::vector<GLuint> *vbos, unsigned int nbPart);
		void			draw_loop(unsigned int nbPart, BaseCl *cl, Camera *camera);
		static void		key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
		static void		mouse_callback(GLFWwindow* window, int button, int action, int mods);
	
};

#endif