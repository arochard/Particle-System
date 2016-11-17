#include <iostream>
#include <fstream>
#include "../includes/Graphic.hpp"
#include "../includes/Exception.hpp"

//PRIVATE

void			Graphic::create_vbo(std::vector<GLuint> *vbos, unsigned int nbPart)
{
	GLuint		tmp;
	const GLfloat vertex_data[] = {
		0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
	};

	glGenVertexArrays(1, &(this->_vao));
	glBindVertexArray(this->_vao);

	//Coord vertice, same for all
	glGenBuffers(1, &tmp);
	vbos->push_back(tmp);
	glBindBuffer(GL_ARRAY_BUFFER, (*vbos)[VERTICE_VBO]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
	//Position particle, different for each object
	glGenBuffers(1, &tmp);
	vbos->push_back(tmp);
	glBindBuffer(GL_ARRAY_BUFFER, (*vbos)[POSITION_VBO]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * nbPart, NULL, GL_DYNAMIC_DRAW);
	//Color paticle, different for each object
	glGenBuffers(1, &tmp);
	vbos->push_back(tmp);
	glBindBuffer(GL_ARRAY_BUFFER, (*vbos)[COLOR_VBO]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLubyte) * 4 * nbPart, NULL,  GL_DYNAMIC_DRAW);

	//Enable different attribut
	//Vertice coord attrib
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, (*vbos)[VERTICE_VBO]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//Position attrib
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, (*vbos)[POSITION_VBO]);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//Color attrib
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, (*vbos)[COLOR_VBO]);
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);

	//Attrib divisor
	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
}

void			Graphic::create_shader()
{
	GLuint			vs;
	GLuint			fs;
	std::string		str;
	const char 		*cstr;

	std::ifstream	vs_file("../Shaders/VertexShader.vs");
	vs_file >> str;
	cstr = str.c_str();
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &(cstr), NULL);

	//Debug
	std::cout << "Vertex Shader : " << str << std::endl;

	str.clear();
	glCompileShader(vs);
	std::ifstream	fs_file("../Shaders/FragmentShader.fs");
	fs_file >> str;
	cstr = str.c_str();
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &(cstr), NULL);

	//Debug
	std::cout << "Frag Shader : " << str << std::endl;

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


void 			Graphic::draw_loop(unsigned int nbPart, BaseCl *cl)
{
	while (!glfwWindowShouldClose(this->_win_ptr))
	{
		this->update_fps_counter();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSwapBuffers(this->_win_ptr);
		glfwPollEvents();
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, nbPart);
		cl->update_position_kernel();
	}
}

//PUBLIC

Graphic::Graphic(int width, int height)
{
	this->init_window(width, height);
	this->create_shader();
}

Graphic::~Graphic(){}