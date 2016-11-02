# include "../includes/Exception.hpp"

Exception::Exception(const char *msg)
{
	std::ostringstream oss; 
	oss << msg; 
	this->_msg = oss.str();
}

Exception::~Exception() throw(){}

const char*	Exception::what() const throw()
{
	return _msg.c_str();
}