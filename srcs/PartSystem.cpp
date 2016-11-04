#include "../includes/PartSystem.hpp"

PartSystem::PartSystem(unsigned int nbPart): _nbPart(nbPart)
{
	this->_base_cl = new BaseCl();
	this->_graphic = new Graphic(WIDTH, HEIGHT);

	this->_graphic->main_loop();
}

PartSystem::~PartSystem(){}