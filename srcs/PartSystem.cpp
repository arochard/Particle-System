#include <iostream>
#include "../includes/PartSystem.hpp"

//PRIVATE

//PUBLIC
PartSystem::PartSystem(unsigned int nbPart): _nbPart(nbPart)
{
	this->_graphic = new Graphic(WIDTH, HEIGHT);
	this->_base_cl = new BaseCl();
	this->_camera = new Camera(WIDTH, HEIGHT);
	this->_graphic->create_vbo(&(this->_vbos), nbPart);
	this->_base_cl->create_buffer(&(this->_vbos), nbPart);
}

PartSystem::~PartSystem(){}

void			PartSystem::main_loop()
{
	this->_base_cl->begin_kernel(1);
	this->_graphic->draw_loop(this->_nbPart, this->_base_cl, this->_camera);
}