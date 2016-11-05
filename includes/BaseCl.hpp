#ifndef BASE_CL_HPP
# define BASE_CL_HPP

# include <string>
# include <iostream>
# include <sstream>
# include <cctype>
# include <vector>
# include "cl.hpp"
# include "Exception.hpp"


class BaseCl
{
	private:
		cl::Platform		_chosen_platform;
		cl::Device			_chosen_device;
		cl::Program 		_program;
		cl::Kernel 			_kernel;
		const std::string	_kernel_source;

		void				platform_select();
		void				program_create();
		void				device_select();

	public:
		BaseCl();
		~BaseCl();
	
};

#endif