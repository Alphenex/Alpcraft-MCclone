#pragma once

#include "GL/glew.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "../../glObj/Shader.h"

class Camera {
public:
	Camera(int width, int height, glm::vec3 position);

	void UpdateMatrix(float fovdeg, float nearplane, float farplane);
	void ShaderMatrix(Shader& shader, const char* uniform);

	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 CamMatrix = glm::mat4(1.0f);

	int Width;
	int Height;

	float Sensitivity = 150.0f;
};