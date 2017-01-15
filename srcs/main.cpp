#include <iostream>
#include <string>
#include "../includes/PartSystem.hpp"
#include "../includes/Exception.hpp"
#include "../includes/utils.hpp"

int					main(int ac, char const *av[])
{
	unsigned int	nbPart;

	try
	{
		if (ac == 2)
		{
			std::string s = std::string(av[1]);
			if (s.length() == 2 && isdigit(s[0]) && (s[1] == 'm' || s[1] == 'M'))
			{
				if (s[1] == 'm')
					nbPart = (s[0] - '0') * 100000;
				else
					nbPart = (s[0] - '0') * 1000000;
				std::cout << nbPart << std::endl;
				PartSystem *ps = new PartSystem(nbPart);
				ps->main_loop();
			}
			else
				throw Exception("Usage: ./ParticleSystem [1-9][m || M]");

		}
		else
			throw Exception("Usage: ./ParticleSystem [1-9][m || M]");
		
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	return 0;
}