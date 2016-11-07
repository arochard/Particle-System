#ifndef BASE_CL_HPP
# define BASE_CL_HPP

# include <string>
# include <iostream>
# include <sstream>
# include <cctype>
# include "Graphic.hpp"
# include "cl.hpp"
# include "Exception.hpp"

# define CL_POS_VBO 0
# define CL_COLOR_VBO 1



class BaseCl
{
	private:
		cl::Platform			_chosen_platform;
		cl::Device				_chosen_device;
		cl::Program 			_program;
		cl::Kernel 				_kernel[2];
		cl::Context 			_context;
		const std::string		_kernel_source;
		cl::Buffer				_cl_velocity;
		std::vector<cl::Memory>	_cl_vbos;
		cl::CommandQueue 		_queue;
		cl::Event 				_event;
		

		void					platform_select();
		void					program_create();
		void					device_select();
		void					set_kernel_args();

	public:
		BaseCl();
		~BaseCl();

		void 					create_buffer(std::vector<GLuint> *vbos, unsigned int nbPart);
		// void 					create_queue(cl::CommandQueue *queue);
	
};

#endif