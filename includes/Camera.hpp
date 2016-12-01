#ifndef CAMERA_HPP
# define CAMERA_HPP

# include "glm/glm.hpp"
# include "glm/gtc/matrix_transform.hpp"

class Camera
{
	private:
		glm::mat4 		_projMat;
		glm::mat4 		_viewMat;

		void 			create_proj_matrix(int width, int height);
		void 			create_view_matrix();
	public:
		Camera(int width, int height);
		~Camera();

		glm::mat4 		getProj();
		glm::mat4 		getView();
	
};

#endif