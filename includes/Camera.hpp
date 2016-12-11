#ifndef CAMERA_HPP
# define CAMERA_HPP

# include "glm/glm.hpp"
# include "glm/gtc/matrix_transform.hpp"

class Camera
{
	private:
		int 			_width;
		int 			_height;
		glm::vec3 		_position;
		float 			_horizontalAngle;
		float 			_verticalAngle;
		float 			_initialFov;
		float 			_speedMouse;
		float 			_speed;
		glm::mat4 		_projMat;
		glm::mat4 		_viewMat;
		glm::mat4		_modelMat;
		glm::mat4 		_mvp;

	public:
		Camera(int width, int height);
		~Camera();

		glm::mat4 		getProj();
		glm::mat4 		getView();
		glm::mat4 		getMVP();
	
};

#endif