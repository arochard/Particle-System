#include "../includes/BaseCl.hpp"

BaseCl::BaseCl()
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

	this->device_select();
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
