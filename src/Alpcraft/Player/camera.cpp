#include "camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
	: Position(position), Width(width), Height(height)
{

}

void Camera::UpdateMatrix(float fovdeg, float nearplane, float farplane)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(Position, Position + Orientation, Up);
	projection = glm::perspective(glm::radians(fovdeg), (float)Width / Height, nearplane, farplane);

	CamMatrix = projection * view;
}
void Camera::ShaderMatrix(Shader& shader, const char* uniform)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(CamMatrix));
}
