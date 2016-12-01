#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include "../includes/Graphic.hpp"
#include "../includes/Exception.hpp"
#include "../includes/utils.hpp"

//PRIVATE

void 			Graphic::send_matrix(Camera *camera)
{
	GLint 		loc;

	glUseProgram(this->_programm_shader);
	loc = glGetUniformLocation(this->_programm_shader, "projMatrix");
	if (loc < -1)
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(camera->getProj()));
	loc = glGetUniformLocation(this->_programm_shader, "viewMatrix");
	if (loc < -1)
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(camera->getView()));

}

void 			Graphic::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{

}

void			Graphic::create_vbo(std::vector<GLuint> *vbos, unsigned int nbPart)
{
	GLuint		tmp;
	// const GLfloat vertex_data[] = {
	// 	0.5f, -0.5f, 0.0f,
	// 	0.5f, -0.5f, 0.0f,
	// 	-0.5f, 0.5f, 0.0f,
	// 	0.5f, 0.5f, 0.0f,
	// };

	glGenVertexArrays(1, &(this->_vao));
	glBindVertexArray(this->_vao);

	//Coord vertice, same for all
	// glGenBuffers(1, &tmp);
	// vbos->push_back(tmp);
	// glBindBuffer(GL_ARRAY_BUFFER, (*vbos)[VERTICE_VBO]);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
	//Position particle, different for each object
	glGenBuffers(1, &tmp);
	vbos->push_back(tmp);
	glBindBuffer(GL_ARRAY_BUFFER, (*vbos)[POSITION_VBO]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * nbPart, NULL, GL_DYNAMIC_DRAW);
	//Color paticle, different for each object
	glGenBuffers(1, &tmp);
	vbos->push_back(tmp);
	glBindBuffer(GL_ARRAY_BUFFER, (*vbos)[COLOR_VBO]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * nbPart, NULL,  GL_DYNAMIC_DRAW);

	//Enable different attribut
	//Vertice coord attrib
	// glEnableVertexAttribArray(0);
	// glBindBuffer(GL_ARRAY_BUFFER, (*vbos)[VERTICE_VBO]);
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//Position attrib
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, (*vbos)[POSITION_VBO]);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDisableVertexAttribArray(0);
	//Color attrib
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, (*vbos)[COLOR_VBO]);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDisableVertexAttribArray(0);
	//Attrib divisor
	// glVertexAttribDivisor(0, 0);
	// glVertexAttribDivisor(1, 1);
	// glVertexAttribDivisor(2, 1);
}

void			Graphic::create_shader()
{
	GLuint			vs;
	GLuint			fs;
	std::string		str;
	const char 		*cstr;

	GLint isCompiled = 0;
	GLint result = GL_FALSE;
    int logLength;

std::cout << glGetError() << std::endl;
	str = read_file("Shaders/VertexShader.vs");
	cstr = str.c_str();
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &cstr, NULL);

	glGetShaderiv(vs, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		std::cout << "Vertex Shader Error : " << glGetError() << std::endl;
	}

	str.clear();
	glCompileShader(vs);


	// Check vertex shader
    glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<GLchar> vertShaderError((logLength > 1) ? logLength : 1);
    glGetShaderInfoLog(vs, logLength, NULL, &vertShaderError[0]);
    std::cout << &vertShaderError[0] << std::endl;


	str = read_file("Shaders/FragmentShader.fs");
	cstr = str.c_str();
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &cstr, NULL);

	glGetShaderiv(fs, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		std::cout << "Fragment Shader Error : " << glGetError() << std::endl;
	}

	glCompileShader(fs);

	// Check frag shader
    glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<GLchar> fragShaderError((logLength > 1) ? logLength : 1);
    glGetShaderInfoLog(vs, logLength, NULL, &fragShaderError[0]);
    std::cout << &fragShaderError[0] << std::endl;


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
	//glViewport(0, 0, width, height);
}

void			Graphic::update_fps_counter()
{
	static double 		prev_sec = glfwGetTime();
	static int 			frame_count;
	double 				cur_sec = glfwGetTime();
	double 				elapse_sec = cur_sec - prev_sec;
	double 				fps;

	if (elapse_sec > 0.15)
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


void 			Graphic::draw_loop(unsigned int nbPart, BaseCl *cl, Camera *camera)
{
	std::vector<double> mouseCoord = {0.0f, 0.0f};
	auto previous_time = std::chrono::steady_clock::now();
	std::chrono::duration<float, std::milli> elapsed;

	GLsizei 	l;
	GLchar 		str[2048];
	std::cout << glGetError() << std::endl;
	glBindVertexArray(this->_vao);
	glUseProgram(this->_programm_shader);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_POINT_SPRITE);
	//glEnable(GL_PROGRAM_POINT_SIZE);
	while (!glfwWindowShouldClose(this->_win_ptr))
	{
		auto current_time = std::chrono::steady_clock::now();
		elapsed = current_time - previous_time;
		send_matrix(camera);
		glfwGetCursorPos(this->_win_ptr, &mouseCoord[0], &mouseCoord[1]);
		// std::cout << mouseCoord[0] << " " << mouseCoord[1] << std::endl;
		glfwSetKeyCallback(this->_win_ptr, this->key_callback);
		this->update_fps_counter();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glfwPollEvents();
		// glGetProgramInfoLog(this->_programm_shader, 2048, &l, str);
		glBindVertexArray(this->_vao);
		// glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, nbPart);
		// glPointSize(20);              //specify size of points in pixels
		glDrawArrays(GL_POINTS, 0, nbPart);
		// glBindVertexArray(0);
		// std::cout << elapsed.count() << std::endl;
		cl->update_position_kernel(std::vector<float>(mouseCoord.begin(), mouseCoord.end()), elapsed.count());
		glfwSwapBuffers(this->_win_ptr);
		previous_time = current_time;
	}
	glUseProgram(0);
	glBindVertexArray(0);
	std::cout << glGetError() << std::endl;
}

//PUBLIC

Graphic::Graphic(int width, int height)
{
	this->init_window(width, height);
	this->create_shader();
}

Graphic::~Graphic(){}