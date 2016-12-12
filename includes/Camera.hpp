#ifndef CAMERA_HPP
# define CAMERA_HPP

# include "glfw3.h"
# include "glm/glm.hpp"
# include "glm/gtc/matrix_transform.hpp"

class Camera
{
	private:
		int 			_width;
		int 			_height;
		glm::vec3 		_position;
		glm::vec3 		_direction;
		glm::vec3		_right;
		glm::vec3 		_up;
		float 			_horizontalAngle;
		float 			_verticalAngle;
		float 			_speedMouse;
		float 			_speed;
		glm::mat4 		_projMat;
		glm::mat4 		_viewMat;
		glm::mat4		_modelMat;
		glm::mat4 		_mvp;

		void 			updateView();

	public:
		Camera(int width, int height);
		~Camera();

		void			setMouseCam(float deltaTime, double posx, double posy);
		void 			up(float deltaTime);
		void 			down(float deltaTime);
		void 			right(float deltaTime);
		void 			left(float deltaTime);

		glm::mat4 		getProj();
		glm::mat4 		getView();
		glm::mat4 		getMVP();
	
};

#endif