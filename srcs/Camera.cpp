#include <cmath>
#include "../includes/Camera.hpp"

//PRIVATE

void 				Camera::updateView()
{
	this->_direction = glm::vec3(cos(this->_verticalAngle) * sin(this->_horizontalAngle),
							sin(this->_verticalAngle),
							cos(this->_verticalAngle) * cos(this->_horizontalAngle));
	this->_right = glm::vec3(sin(this->_horizontalAngle - 3.14f / 2.0f),
						0,
						cos(this->_horizontalAngle - 3.14f / 2.0f));
	this->_up = glm::cross(this->_right, this->_direction);
	// glFinish();
	this->_viewMat = glm::lookAt(this->_position, this->_position + this->_direction, this->_up);
	this->_mvp = this->_projMat * this->_viewMat * this->_modelMat;
}


//PUBLIC
Camera::Camera(int width, int height): _width(width), _height(height)
{
	this->_position = glm::vec3(0, 0, 0.5);
	this->_horizontalAngle = 3.14f;
	this->_verticalAngle = 0.0f;
	this->_speed = 3.0;
	this->_speedMouse = 0.005f;
	this->_position = glm::vec3(0, 0, 0);
	this->_viewMat = glm::lookAt(glm::vec3(4.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
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

void				Camera::setMouseCam(float deltaTime, double posx, double posy)
{
	this->_horizontalAngle += this->_speedMouse * deltaTime * (float)(this->_width / 2 - posx);
	this->_verticalAngle += this->_speedMouse * deltaTime * (float)(this->_height / 2 - posy);
	this->updateView();
}

void 				Camera::up(float deltaTime)
{
	this->_position += this->_direction * deltaTime * this->_speed;
	this->updateView();
}

void 				Camera::down(float deltaTime)
{
	this->_position -= this->_direction * deltaTime * this->_speed;
	this->updateView();
}

void 				Camera::right(float deltaTime)
{
	this->_position += this->_right * deltaTime * this->_speed;
	this->updateView();
}

void 				Camera::left(float deltaTime)
{
	this->_position -= this->_right * deltaTime * this->_speed;
	this->updateView();
}