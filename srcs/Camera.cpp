#include "../includes/Camera.hpp"

//PRIVATE
void 			Camera::create_view_matrix()
{
	this->_viewMat = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.0f, 0.0f));
}

void 			Camera::create_proj_matrix(int width, int height)
{
	this->_projMat = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
}

//PUBLIC
Camera::Camera(int width, int height)
{
	create_proj_matrix(width, height);
	create_view_matrix();
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