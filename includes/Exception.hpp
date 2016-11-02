#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

# include <exception>
# include <string>
# include <sstream>

class Exception : public std::exception
{
	private:
		std::string		_msg;

	public:
		Exception(const char *msg);
		virtual ~Exception() throw();
		virtual const char* what() const throw();
	
};


#endif 