#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <ratio>
#include "../includes/Graphic.hpp"
#include "../includes/Exception.hpp"
#include "../includes/utils.hpp"

//PRIVATE

float 			Graphic::_deltaTime = 0.0f;
Camera 			*Graphic::_camera_ptr = NULL;
bool 			Graphic::_button_pressed = false;
int 			Graphic::_grav_actived = 0;
unsigned int 	Graphic::_begin_form = 0;


void 			Graphic::send_matrix()
{
	GLint 		loc;

	glUseProgram(this->_programm_shader);
	loc = glGetUniformLocation(this->_programm_shader, "MVP");
	if (loc > -1)
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(this->_camera->getMVP()));
}

void			Graphic::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	mods = 0;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		_button_pressed = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		_button_pressed = false;
	}
}

void 			Graphic::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	window = NULL;
	mods = 0;
	scancode = 0;
	_grav_actived = 0;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		exit(0);
	if (key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS))
		_camera_ptr->right(_deltaTime);
	if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS))
		_camera_ptr->left(_deltaTime);
	if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
		_camera_ptr->up(_deltaTime);
	if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
		_camera_ptr->down(_deltaTime);
	if (key == GLFW_KEY_LEFT_CONTROL && (action == GLFW_REPEAT || action == GLFW_PRESS))
		_grav_actived = 1;
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		_begin_form = 1;
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		_begin_form = 2;
}

void			Graphic::create_vbo(std::vector<GLuint> *vbos, unsigned int nbPart)
{
	GLuint		tmp;

	glGenVertexArrays(1, &(this->_vao));
	glBindVertexArray(this->_vao);
	glGenBuffers(1, &tmp);
	vbos->push_back(tmp);
	glBindBuffer(GL_ARRAY_BUFFER, (*vbos)[POSITION_VBO]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * nbPart, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, (*vbos)[POSITION_VBO]);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glGenBuffers(1, &tmp);
	vbos->push_back(tmp);
	glBindBuffer(GL_ARRAY_BUFFER, (*vbos)[COLOR_VBO]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * nbPart, NULL,  GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, (*vbos)[COLOR_VBO]);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void			Graphic::create_shader()
{
	GLuint			vs;
	GLuint			fs;
	std::string		str;
	const char 		*cstr;

	str = read_file("Shaders/VertexShader.vs");
	cstr = str.c_str();
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &cstr, NULL);
	str.clear();
	glCompileShader(vs);
	str = read_file("Shaders/FragmentShader.fs");
	cstr = str.c_str();
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &cstr, NULL);
	glCompileShader(fs);
	this->_programm_shader = glCreateProgram();
	glAttachShader(this->_programm_shader, fs);
	glAttachShader(this->_programm_shader, vs);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glLinkProgram(this->_programm_shader);
	glUseProgram(0);
}

void			Graphic::init_window(int width, int height)
{
	if (!glfwInit())
		throw Exception("ERROR: could not start GLFW3\n");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	this->_win_ptr = glfwCreateWindow(width, height, "Particle System", NULL, NULL);
	if (!this->_win_ptr)
	{
		glfwTerminate();
		throw Exception("ERROR: could not open window with GLFW3\n");
	}
	glfwMakeContextCurrent(this->_win_ptr);
	glewExperimental = GL_TRUE;
	glfwSwapInterval(0);
	glewInit();
	glViewport(0, 0, width, height);
}

void			Graphic::update_fps_counter()
{
	static double 		prev_sec = glfwGetTime();
	static int 			frame_count;
	double 				cur_sec = glfwGetTime();
	double 				elapse_sec = cur_sec - prev_sec;
	double 				fps;

	if (elapse_sec > 0.5)
	{
		prev_sec = cur_sec;
		fps = (double)frame_count / elapse_sec;
		std::ostringstream os;
		os << "Particle System ; FPS : " << fps;
		glfwSetWindowTitle(this->_win_ptr, os.str().c_str());
		frame_count = 0;
	}
	frame_count++;
}

void 			Graphic::ray_picking(std::vector<double>  &mouse)
{
	mouse[0] = (2.0f * mouse[0]) / this->_width - 1.0f;
	mouse[1] = 1.0f - (2.0f * mouse[1]) / this->_height;
	glm::vec4 ray_wor = this->_camera->getView() * glm::vec4(mouse[0], mouse[1], -1.0f, 1.0f);
	mouse[0] = ray_wor.x;
	mouse[1] = ray_wor.y;
	mouse[2] = ray_wor.z;
}

void 			Graphic::draw_loop(unsigned int nbPart, BaseCl *cl, Camera *camera)
{
	std::vector<double> mouseCoord = {0.0f, 0.0f};
	std::vector<double> mouseCoordGrav = {0.0f, 0.0f, 0.0f, 0.0f};
	typedef std::chrono::high_resolution_clock Time;
	auto prev_time = Time::now();
	auto cur_time = Time::now();
	std::chrono::duration<double> time_span;
	int grav = 0;

	this->_camera = camera;
	_camera_ptr = this->_camera;
	glBindVertexArray(this->_vao);
	glUseProgram(this->_programm_shader);
	glEnable(GL_DEPTH);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SPRITE);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glfwSetMouseButtonCallback(this->_win_ptr, this->mouse_callback);
	glfwSetKeyCallback(this->_win_ptr, this->key_callback);

	while (!glfwWindowShouldClose(this->_win_ptr))
	{
		cur_time = Time::now();
		time_span = std::chrono::duration_cast<std::chrono::duration<float>>(cur_time - prev_time);
		prev_time = Time::now();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
		glfwGetCursorPos(this->_win_ptr, &mouseCoord[0], &mouseCoord[1]);
		if (_begin_form)
		{
			cl->begin_kernel(_begin_form);
			_begin_form = 0;
			grav = 0;
			_grav_actived = 0;
		}
		if (_button_pressed)
			this->_camera->setMouseCam(_deltaTime, mouseCoord[0], mouseCoord[1]);
		else if (_grav_actived)
		{
			mouseCoordGrav[0] = mouseCoord[0];
			mouseCoordGrav[1] = mouseCoord[1];
			ray_picking(mouseCoordGrav);
			grav = 1;
		}
		send_matrix();
		this->update_fps_counter();
		glDrawArrays(GL_POINTS, 0, nbPart);
		cl->update_position_kernel(std::vector<float> (mouseCoordGrav.begin(), mouseCoordGrav.end()), time_span.count(), grav);
		glfwSwapBuffers(this->_win_ptr);
		_deltaTime = time_span.count();
	}
	glUseProgram(0);
	glBindVertexArray(0);
}

//PUBLIC

Graphic::Graphic(int width, int height): _width(width), _height(height)
{
	this->init_window(width, height);
	this->create_shader();
}

Graphic::~Graphic(){}