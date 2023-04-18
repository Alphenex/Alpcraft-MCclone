#include "camera.h"
#include "../AWorld.h"
#include <SFML/Graphics.hpp>

class Player {
public:
	Player(glm::vec3 _StartPosition);
	~Player();

	void HandleMouseInput(World& world, sf::Window& window);
	void HandleMovement(sf::Window& window, float dt, bool noclip);

	void Update(sf::Window& window, float dt, bool noclip, Shader& shader);

	void Render(World& world);

	glm::vec3 GetPosition() { return m_Position; }

private:
	Camera* m_View;

	glm::vec3 m_Position;

	float c_FOV = 70.0f;

	float c_MovementSpeed = 20.0f;
	float c_FlightSpeed = 10.0f;

	// Alpcraft Related Stoof //

	VAO m_VAO;
	Shader* m_Shader;

	Block m_SelectedBlock = Grass;
};