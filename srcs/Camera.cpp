#include "../includes/Camera.hpp"

//PRIVATE
void 			Camera::create_view_matrix(int width, int height)
{
	this->_viewMat = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
}

//PUBLIC
Camera::Camera(int width, int height)
{
	create_view_matrix(width, height);

}

Camera::~Camera(){}

glm::mat4 			Camera::getView()
{
	return (this->_viewMat);
}