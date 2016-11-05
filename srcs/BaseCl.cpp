#include "../includes/BaseCl.hpp"

BaseCl::BaseCl()
{
	this->platform_select();
	this->device_select();
	this->program_create();
}

BaseCl::~BaseCl(){}


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
	std::vector<cl::Device> device_vector = {this->_chosen_device};
	cl::Context 			context(device_vector);
	cl::Program::Sources	sources = {{this->_kernel_source.c_str(), this->_kernel_source.length()}};

	this->_program = cl::Program(context, sources);
	if (this->_program.build(device_vector) != CL_SUCCESS)
	{
		std::cerr << this->_program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(this->_chosen_device) << std::endl;
		throw Exception("Error building program");
	}

	this->_kernel = cl::Kernel(this->_program, "myKernel");
}
