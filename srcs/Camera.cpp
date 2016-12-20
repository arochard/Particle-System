#include <cmath>
#include <iostream>
#include "../includes/Camera.hpp"

//PRIVATE

void				Camera::updateView(float deltaTime)
{
	//DEBUG
	// std::cout << deltaTime << std::endl;

	glm::vec3 forward(this->_viewMat[0][2], this->_viewMat[1][2], this->_viewMat[2][2]);
	glm::vec3 strafe( this->_viewMat[0][0], this->_viewMat[1][0], this->_viewMat[2][0]);
	const float speed = 5.0f;
	eyeVector += (-_dz * forward + _dx * strafe) * speed * deltaTime;
	_dz = _dx = 0;
	glm::quat key_quat = glm::quat(glm::vec3(key_pitch, key_yaw, key_roll));
	key_pitch = key_yaw = key_roll = 0;
	camera_quat = key_quat * camera_quat;
	camera_quat = glm::normalize(camera_quat);
	glm::mat4 rotate = glm::mat4_cast(camera_quat);
	glm::mat4 translate = glm::mat4(1.0f);
	translate = glm::translate(translate, -(this->eyeVector));
	this->_viewMat = rotate * translate;
	this->_mvp = this->_projMat * this->_viewMat * this->_modelMat;
}


//PUBLIC
Camera::Camera(int width, int height): _width(width), _height(height)
{
	this->_viewMat = glm::lookAt(glm::vec3(4.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	this->_projMat = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
	this->_modelMat = glm::mat4(1.0f);
	this->mousePosition = glm::vec2(0, 0);
	this->camera_quat = glm::quat(glm::vec3(0, 0, 5));
	_dz = _dx = key_pitch = key_yaw = key_roll = 0;
	this->_mvp = this->_projMat * this->_viewMat * this->_modelMat;
	this->updateView(1.0);

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
	glm::vec2 mouse_delta = glm::vec2(posx, posy) - mousePosition;
	const float mouseX_Sensitivity = 0.40f;
	const float mouseY_Sensitivity = 0.40f;
	key_yaw   = mouseX_Sensitivity * mouse_delta.x * deltaTime;
	key_pitch = mouseY_Sensitivity * mouse_delta.y * deltaTime;
	mousePosition = glm::vec2(posx, posy);
	this->updateView(deltaTime);
}

void 				Camera::up(float deltaTime)
{
	this->_dz = PAD;
	this->updateView(deltaTime);
}

void 				Camera::down(float deltaTime)
{
	this->_dz = -PAD;
	this->updateView(deltaTime);
}

void 				Camera::right(float deltaTime)
{
	this->_dx = PAD;
	this->updateView(deltaTime);
}

void 				Camera::left(float deltaTime)
{
	this->_dx = -PAD;
	this->updateView(deltaTime);
}