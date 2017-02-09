#ifndef CAMERA_HPP
# define CAMERA_HPP

# include "glfw3.h"
# include "glm/glm.hpp"
# include "glm/gtc/matrix_transform.hpp"
# include "glm/gtc/quaternion.hpp"

# define PAD 2

class Camera
{
	private:
		glm::mat4 		_projMat;
		glm::mat4 		_viewMat;
		glm::mat4		_modelMat;
		glm::mat4 		_mvp;
		glm::quat		camera_quat;
		float 			key_pitch;
		float 			key_yaw;
		float 			key_roll;
		glm::vec2 		mousePosition;
		glm::vec3 		eyeVector;
		float 			_dx;
		float 			_dz;

		void 			updateView(float deltaTime);

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