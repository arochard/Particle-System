#include "../includes/Camera.hpp"

//PRIVATE


//PUBLIC
Camera::Camera(int width, int height): _width(width), _height(height)
{
	this->_position = glm::vec3(0, 0, 0.5);
	this->_horizontalAngle = 3.14f;
	this->_verticalAngle = 0.0f;
	this->_initialFov = 45.0f;
	this->_speed = 3.0;
	this->_speedMouse = 0.5f;
	this->_viewMat = glm::lookAt(glm::vec3(4, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	this->_projMat = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 10.0f);
	this->_modelMat = glm::mat4(1.0f);
	this->_mvp = this->_projMat * this->_viewMat * this->_modelMat;
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

void				Camera::setMouseCam(GLFWwindow* window)
{
	int x;
	int y;

	glfwGetCursorPos(window, &x, &y);
	glfwSetCursorPos(window, this->_width / 2, this->_height / 2);

	this->_horizontalAngle += this->_speedMouse * //deltatime

}