#include <cmath>
#include <iostream>
#include "../includes/Camera.hpp"

//PRIVATE

void				Camera::updateView(float deltaTime)
{
	glm::vec3 forward(this->_viewMat[0][2], this->_viewMat[1][2], this->_viewMat[2][2]);
	glm::vec3 strafe( this->_viewMat[0][0], this->_viewMat[1][0], this->_viewMat[2][0]);
	const float speed = 5.0f;
	this->eyeVector += (-_dz * forward + _dx * strafe) * speed * deltaTime;
	this->_dz = this->_dx = 0;
	glm::quat key_quat = glm::quat(glm::vec3(this->key_pitch, this->key_yaw, this->key_roll));
	this->key_pitch = this->key_yaw = this->key_roll = 0;
	this->camera_quat = key_quat * this->camera_quat;
	this->camera_quat = glm::normalize(camera_quat);
	glm::mat4 rotate = glm::mat4_cast(camera_quat);
	glm::mat4 translate = glm::mat4(1.0f);
	translate = glm::translate(translate, -(this->eyeVector));
	this->_viewMat = rotate * translate;
	this->_mvp = this->_projMat * this->_viewMat * this->_modelMat;	
}


//PUBLIC
Camera::Camera(int width, int height)
{
	this->_viewMat = glm::mat4(1);
	this->_projMat = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
	this->_modelMat = glm::mat4();
	this->mousePosition = glm::vec2(0, 0);
	this->camera_quat = glm::quat(glm::vec3(0, 0, 0));
	this->_dx = 0;
	this->_dz = -0.3;
	this->key_pitch = 0;
	this->key_yaw = 0;
	this->key_roll = 0;
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
	if (mouse_delta.x < 40 && mouse_delta.x > -40 && mouse_delta.y > -40 && mouse_delta.y < 40)
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