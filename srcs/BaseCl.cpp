#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include "../includes/Graphic.hpp"
#include "../includes/BaseCl.hpp"
#include "../includes/Exception.hpp"
#include "../includes/utils.hpp"

//PUBLIC
BaseCl::BaseCl()
{
	this->platform_select();
	this->device_select();
	this->program_create();
}

BaseCl::~BaseCl(){}

void		BaseCl::create_buffer(std::vector<GLuint> *vbos, unsigned int nbPart)
{
	cl_int	err;

	this->_numPart = nbPart;

	glFinish();
	//Debug
	std::cout << "Opengl interop" << std::endl;
	this->_cl_vbos.push_back(cl::BufferGL(this->_context, CL_MEM_READ_WRITE, (*vbos)[POSITION_VBO], &err));
	this->_cl_vbos.push_back(cl::BufferGL(this->_context, CL_MEM_READ_WRITE, (*vbos)[COLOR_VBO], &err));
	this->_cl_velocity = cl::Buffer(this->_context, CL_MEM_READ_WRITE, 4 * sizeof(float) * nbPart, NULL, &err);
	this->_queue = cl::CommandQueue(this->_context, this->_chosen_device, 0, &err);
	this->_queue.finish();
	this->set_kernel_args(nbPart);

	//Debug
	std::cout << "End opengl interop" << std::endl;
}

void 		BaseCl::update_position_kernel(std::vector<float> mouse)
{
	cl_int 	err;

	try
	{
		glFinish();

		this->_queue.enqueueAcquireGLObjects(&(this->_cl_vbos), NULL, &(this->_event));
		this->_queue.finish();

		err = this->_kernel[UPDATE_KERNEL].setArg(3, sizeof(cl_float2), &mouse);

		this->_queue.enqueueNDRangeKernel(this->_kernel[UPDATE_KERNEL], cl::NullRange, cl::NDRange(this->_numPart), cl::NullRange, NULL, &(this->_event));
		this->_queue.finish();

		this->_queue.enqueueReleaseGLObjects(&(this->_cl_vbos), NULL, &(this->_event));
		this->_queue.finish();
	}
	catch (cl::Error er)
	{
		std::cout << er.err() << std::endl;
		throw Exception(er.what() + er.err());
	}

}

void		BaseCl::begin_kernel()
{
	try
	{
		glFinish();

		this->_queue.enqueueAcquireGLObjects(&(this->_cl_vbos), NULL, &(this->_event));
		this->_queue.finish();

		this->_queue.enqueueNDRangeKernel(this->_kernel[BEGIN_KERNEL], cl::NullRange, cl::NDRange(this->_numPart), cl::NullRange, NULL, &(this->_event));
		this->_queue.finish();

		this->_queue.enqueueReleaseGLObjects(&(this->_cl_vbos), NULL, &(this->_event));
		this->_queue.finish();
	}
	catch (cl::Error er)
	{
		std::cout << er.err() << std::endl;
		throw Exception(er.what() + er.err());
	}

}


//PRIVATE

void 		BaseCl::create_context(std::vector<cl::Device> *device)
{
	CGLContextObj kCGLContext = CGLGetCurrentContext();
	CGLShareGroupObj kCGLShareGroup = CGLGetShareGroup(kCGLContext);
	cl_context_properties props[] =
	{
		CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)kCGLShareGroup,
		0
	};

	try
	{
		this->_context = cl::Context(*device, props);
	}
	catch (cl::Error er)
	{
		throw Exception(er.what() + er.err());
	}
}

void		BaseCl::set_kernel_args(unsigned int nbPart)
{
	cl_int 		err;
	float		pad = 0.5f / nbPart;
	std::vector<float> mouse = {0.0f, 0.0f};

	//update position
	err = this->_kernel[0].setArg(0, this->_cl_vbos[CL_POS_VBO]);
	err = this->_kernel[0].setArg(1, this->_cl_vbos[CL_COLOR_VBO]);
	err = this->_kernel[0].setArg(2, this->_cl_velocity);
	//postition begin
	err = this->_kernel[1].setArg(0, this->_cl_vbos[CL_POS_VBO]);
	err = this->_kernel[1].setArg(1, this->_cl_vbos[CL_COLOR_VBO]);
	err = this->_kernel[1].setArg(2, this->_cl_velocity);
	err = this->_kernel[1].setArg(3, sizeof(float), &pad);
	this->_queue.finish();
}

void		BaseCl::device_select()
{
	std::vector<cl::Device> 	devices;

	this->_chosen_platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);

	if (devices.size() == 0)
		throw Exception("No device found");

	//Debug
	for (auto& dev : devices)
		std::cout << "Available device : " << dev.getInfo<CL_DEVICE_NAME>() << std::endl;

	this->_chosen_device = devices[0];

	//Debug
	std::cout << "Using device : " << this->_chosen_device.getInfo<CL_DEVICE_NAME>() << std::endl;
}

void		BaseCl::platform_select()
{
	std::vector<cl::Platform> platforms;

	cl::Platform::get(&platforms);
	if (platforms.size() == 0)
		throw Exception("No platforms available");

	//Debug
	for (auto& plat : platforms)
		std::cout << "Available platform : " << plat.getInfo<CL_PLATFORM_NAME>() << std::endl;

	this->_chosen_platform = platforms[0];

	//Debug
	std::cout << "Using platform : " << this->_chosen_platform.getInfo<CL_PLATFORM_NAME>() << std::endl;
}

void		BaseCl::program_create()
{
	std::string		str = read_file("Kernel_prog/myKernel.cl");
	std::vector<cl::Device> device_vector = {this->_chosen_device};

	this->create_context(&device_vector);
	cl::Program::Sources	sources = {{str.c_str(), str.length()}};
	try
	{
		cl_int err;
		this->_program = cl::Program(this->_context, sources);
		this->_program.build(device_vector, "-cl-std=CL1.2");
		this->_kernel[0] = cl::Kernel(this->_program, "update_position", &err);
		this->_kernel[1] = cl::Kernel(this->_program, "position_begin", &err);
		//DEBUG
		std::cout << this->_kernel[1].getInfo<CL_KERNEL_REFERENCE_COUNT>() << std::endl;
	}
	catch (cl::Error er)
	{
		std::cout << er.err() << std::endl;
		std::cerr << this->_program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(this->_chosen_device) << std::endl;
		throw Exception(er.what() + er.err());
	}

	//Debug
	std::cout << "Status" << std::endl;
	std::cout << "Build Status: " << this->_program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(device_vector[0]) << std::endl;
	std::cout << "Build Options:\t" << this->_program.getBuildInfo<CL_PROGRAM_BUILD_OPTIONS>(device_vector[0]) << std::endl;
	std::cout << "Build Log:\t " << this->_program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device_vector[0]) << std::endl;
}
