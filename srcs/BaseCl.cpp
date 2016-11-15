#include "../includes/BaseCl.hpp"

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

	// glFinish();
	//Debug
	std::cout << "Opengl interop" << std::endl;
	// this->_cl_vbos.push_back(cl::BufferGL(this->_context, CL_MEM_READ_WRITE, (*vbos)[VERTICE_VBO], &err));
	//Debug
	this->_cl_vbos.push_back(cl::BufferGL(this->_context, CL_MEM_READ_WRITE, (*vbos)[POSITION_VBO], &err));
	//Debug
	std::cout << err << std::endl;
	this->_cl_vbos.push_back(cl::BufferGL(this->_context, CL_MEM_READ_WRITE, (*vbos)[COLOR_VBO], &err));
	//Debug
	std::cout << err << std::endl;

	this->_cl_velocity = cl::Buffer(this->_context, CL_MEM_READ_WRITE, 4 * sizeof(float) * nbPart, NULL, &err);
	//debug
	std::cout << err << std::endl;
	this->_queue = cl::CommandQueue(this->_context, this->_chosen_device, 0, &err);
	//debug
	std::cout << err << std::endl;
	err = this->_queue.enqueueWriteBuffer(this->_cl_velocity, CL_TRUE, 0, 4 * sizeof(float) * nbPart, 0, NULL, &(this->_event));
	this->_queue.finish();

	this->set_kernel_args(nbPart);
}


//PRIVATE

void		BaseCl::set_kernel_args(unsigned int nbPart)
{
	cl_int 		err;
	float		pad = 0.5f / nbPart;
	std::vector<float> mouse = {0.0f, 0.0f};

	try
	{
		//update position
		err = this->_kernel[0].setArg(0, this->_cl_vbos[CL_POS_VBO]);
		err = this->_kernel[0].setArg(1, this->_cl_vbos[CL_COLOR_VBO]);
		err = this->_kernel[0].setArg(2, this->_cl_velocity);
		err = this->_kernel[0].setArg(3, &mouse);
		//postition begin
		err = this->_kernel[1].setArg(0, this->_cl_vbos[CL_POS_VBO]);
		err = this->_kernel[1].setArg(1, this->_cl_velocity);
		err = this->_kernel[1].setArg(2, &pad);

	}
	catch (cl::Error er)
	{
		throw Exception(er.what());
	}

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
	std::vector<cl::Device> device_vector = {this->_chosen_device};
	this->_context = cl::Context(device_vector);
	cl::Program::Sources	sources = {{this->_kernel_source.c_str(), this->_kernel_source.length()}};

	this->_program = cl::Program(this->_context, sources);
	if (this->_program.build(device_vector) != CL_SUCCESS)
	{
		std::cerr << this->_program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(this->_chosen_device) << std::endl;
		throw Exception("Error building program");
	}
	try
	{
		this->_kernel[0] = cl::Kernel(this->_program, "update_position");
		this->_kernel[1] = cl::Kernel(this->_program, "position_begin");
	}
	catch (cl::Error er)
	{
		throw Exception(er.what());
	}

}
