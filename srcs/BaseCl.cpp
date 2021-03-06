#include <string>
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <ctime>
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
	this->_cl_vbos.push_back(cl::BufferGL(this->_context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, (*vbos)[POSITION_VBO], &err));
	this->_cl_vbos.push_back(cl::BufferGL(this->_context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, (*vbos)[COLOR_VBO], &err));
	this->_cl_velocity = cl::Buffer(this->_context, CL_MEM_READ_WRITE, 4 * sizeof(float) * nbPart, NULL, &err);
	this->_queue = cl::CommandQueue(this->_context, this->_chosen_device, 0, &err);
	this->_queue.finish();
	this->set_kernel_args(nbPart);
}

void 		BaseCl::update_position_kernel(std::vector<float> mouse, float dt, int grav)
{
	try
	{
		glFinish();
		this->_queue.enqueueAcquireGLObjects(&(this->_cl_vbos), NULL, &(this->_event));
		this->_queue.finish();
		this->_kernel[UPDATE_KERNEL].setArg(4, sizeof(cl_float), &mouse[0]);
		this->_kernel[UPDATE_KERNEL].setArg(5, sizeof(cl_float), &mouse[1]);
		this->_kernel[UPDATE_KERNEL].setArg(6, sizeof(cl_float), &mouse[2]);
		this->_kernel[UPDATE_KERNEL].setArg(7, sizeof(cl_float), &dt);
		this->_kernel[UPDATE_KERNEL].setArg(8, sizeof(int), &grav);

		this->_queue.enqueueNDRangeKernel(this->_kernel[UPDATE_KERNEL], cl::NullRange, cl::NDRange(this->_numPart + (this->_workgroup_size - (this->_numPart % this->_workgroup_size))), cl::NullRange, NULL, &(this->_event));
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

void		BaseCl::begin_kernel(unsigned int type)
{
	cl::NDRange range(this->_numPart + (this->_workgroup_size - (this->_numPart % this->_workgroup_size)));
	cl_uint seed = static_cast<cl_uint>(time(NULL));

	try
	{
		glFinish();
		this->_queue.enqueueAcquireGLObjects(&(this->_cl_vbos), NULL, &(this->_event));
		this->_queue.finish();

		this->_kernel[BEGIN_KERNEL].setArg(4, sizeof(cl_uint), &type);
		this->_kernel[BEGIN_KERNEL].setArg(5, sizeof(cl_uint), &seed);

		this->_queue.enqueueNDRangeKernel(this->_kernel[BEGIN_KERNEL], cl::NullRange, range, cl::NullRange, NULL, &(this->_event));
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
	std::vector<float> mouse = {0.0f, 0.0f};

	//update position
	err = this->_kernel[UPDATE_KERNEL].setArg(0, sizeof(cl_mem), &(this->_cl_vbos[CL_POS_VBO]));
	err = this->_kernel[UPDATE_KERNEL].setArg(1, sizeof(cl_mem), &(this->_cl_vbos[CL_COLOR_VBO]));
	err = this->_kernel[UPDATE_KERNEL].setArg(2, sizeof(cl_mem), &(this->_cl_velocity));
	err = this->_kernel[UPDATE_KERNEL].setArg(3, sizeof(cl_int), &nbPart);
	//postition begin
	err = this->_kernel[BEGIN_KERNEL].setArg(0, sizeof(cl_mem), &(this->_cl_vbos[CL_POS_VBO]));
	err = this->_kernel[BEGIN_KERNEL].setArg(1, sizeof(cl_mem), &(this->_cl_vbos[CL_COLOR_VBO]));
	err = this->_kernel[BEGIN_KERNEL].setArg(2, sizeof(cl_mem), &(this->_cl_velocity));
	err = this->_kernel[BEGIN_KERNEL].setArg(3, sizeof(cl_int), &nbPart);
	this->_queue.finish();
}

void		BaseCl::device_select()
{
	std::vector<cl::Device> 	devices;

	this->_chosen_platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);

	if (devices.size() == 0)
		throw Exception("No device found");

	this->_chosen_device = devices[0];
	this->_workgroup_size = this->_chosen_device.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>()[0];

}

void		BaseCl::platform_select()
{
	std::vector<cl::Platform> platforms;

	cl::Platform::get(&platforms);
	if (platforms.size() == 0)
		throw Exception("No platforms available");
	this->_chosen_platform = platforms[0];
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
		this->_kernel[UPDATE_KERNEL] = cl::Kernel(this->_program, "update_position", &err);
		this->_kernel[BEGIN_KERNEL] = cl::Kernel(this->_program, "position_begin", &err);
	}
	catch (cl::Error er)
	{
		std::cout << er.err() << std::endl;
		std::cerr << this->_program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(this->_chosen_device) << std::endl;
		throw Exception(er.what() + er.err());
	}
}
