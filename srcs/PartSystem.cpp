#include <iostream>
#include "../includes/PartSystem.hpp"

//PRIVATE

//PUBLIC
PartSystem::PartSystem(unsigned int nbPart): _nbPart(nbPart)
{
	this->_graphic = new Graphic(WIDTH, HEIGHT);
	this->_base_cl = new BaseCl();
	this->_graphic->create_vbo(&(this->_vbos), nbPart);
	this->_base_cl->create_buffer(&(this->_vbos), nbPart);
}

PartSystem::~PartSystem(){}

void			PartSystem::main_loop()
{
	std::cout << "Main loop" << std::endl;
	this->_base_cl->begin_kernel();
	this->_graphic->draw_loop(this->_nbPart, this->_base_cl);
}