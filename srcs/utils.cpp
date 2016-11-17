#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <cerrno>
#include <cstdio>
#include "../includes/utils.hpp"

std::string			read_file(const char *path)
{
	std::ifstream infile {path};
	std::string str { std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>() };
    std::cout << str.length() << std::endl;
	return (str);
}