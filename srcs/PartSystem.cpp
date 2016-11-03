#include "../includes/PartSystem.hpp"

PartSystem::PartSystem(unsigned int nbPart): _nbPart(nbPart)
{
	this->_base_cl = new BaseCl();
}

PartSystem::~PartSystem(){}