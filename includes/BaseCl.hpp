#ifndef BASE_CL_HPP
# define BASE_CL_HPP
#define __CL_ENABLE_EXCEPTIONS

# include <OpenCL/opencl.h>
# include <OpenGL/OpenGL.h>
# include "cl.hpp"

# define CL_POS_VBO 0
# define CL_COLOR_VBO 1

# define UPDATE_KERNEL 0
# define BEGIN_KERNEL 1


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
		unsigned int			_numPart;
		

		void					platform_select();
		void					program_create();
		void					device_select();
		void					set_kernel_args(unsigned int nbPart);
		void 					create_context(std::vector<cl::Device> *device);

	public:
		BaseCl();
		~BaseCl();

		void 					create_buffer(std::vector<GLuint> *vbos, unsigned int nbPart);
		void 					update_position_kernel(std::vector<float> mouse, float dt);
		void					begin_kernel();
	
};

#endif