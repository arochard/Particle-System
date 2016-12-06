#include "../includes/Camera.hpp"

//PRIVATE


//PUBLIC
Camera::Camera(int width, int height)
{
	this->_viewMat = glm::lookAt(glm::vec3(4, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	this->_projMat = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 10.0f);
	//this->_mvp = this->_projMat * this->_viewMat;
}

Camera::~Camera(){}

glm::mat4 			Camera::getProj()
{
	return (this->_projMat);
}

glm::mat4 			Camera::getView()
{
	return (this->_viewMat);
}

glm::mat4 			Camera::getMVP()
{
	return (this->_mvp);
}