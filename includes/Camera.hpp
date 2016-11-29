#ifndef CAMERA_HPP
# define CAMERA_HPP

# include "glm/glm.hpp"
# include "glm/gtc/matrix_transform.hpp"

class Camera
{
	private:
		glm::mat4 		_viewMat;

		void 			create_view_matrix(int width, int height);
	public:
		Camera(int width, int height);
		~Camera();

		glm::mat4 		getView();
	
};

#endif