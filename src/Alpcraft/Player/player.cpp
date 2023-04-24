#include "player.h"
#include "../../gameinfo.h"

Player::Player(glm::vec3 _StartPosition)
	: m_Position(_StartPosition)
{
	// Init the camera.
	m_View = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, _StartPosition);
}

Player::~Player()
{
	delete m_View;
}

void Player::HandleMouseInput(World& world, sf::Window& window)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && window.hasFocus())
	{
		for (float i = 0; i < 256; i += 0.1f)
		{
			glm::vec3 AimedBlock = m_View->Position + (m_View->Orientation * i);
			if (world.GetWorldBlock(AimedBlock) != Air)
			{
				if (m_SelectedBlock != Air)
				{
					glm::vec3 checkpos = AimedBlock - m_View->Orientation * 0.1f;

					world.SetWorldBlock(checkpos, m_SelectedBlock);

					world.UpdateChunkWNeighbours(checkpos);
					break;
				}
			}
		}
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && window.hasFocus())
	{
		for (float i = 0; i < 256; i += 0.1f)
		{
			glm::vec3 AimedBlock = m_Position + (m_View->Orientation * i);
			if (world.GetWorldBlock(AimedBlock) != Air)
			{
				world.RemoveWorldBlock(AimedBlock);

				world.UpdateChunkWNeighbours(AimedBlock);
				break;
			}
		}
	}
}

void Player::HandleMovement(sf::Window& window, float dt, bool noclip)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		m_Position += c_MovementSpeed * m_View->Orientation;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		m_Position += c_MovementSpeed * -m_View->Orientation;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_Position += c_MovementSpeed * -glm::normalize(glm::cross(m_View->Orientation, m_View->Up));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_Position += c_MovementSpeed * glm::normalize(glm::cross(m_View->Orientation, m_View->Up));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		c_MovementSpeed = 50.0f * dt;
		c_FlightSpeed = 20.0f * dt;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
	{
		c_MovementSpeed = 5.0f * dt;
		c_FlightSpeed = 5.0f * dt;
	}
	else
	{
		c_MovementSpeed = 20.0f * dt;
		c_FlightSpeed = 10.0f * dt;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		m_Position += c_FlightSpeed * m_View->Up;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
	{
		m_Position += c_FlightSpeed * -m_View->Up;
	}

	m_View->Position = m_Position;

	if (window.hasFocus())
	{
		window.setMouseCursorVisible(false);

		double mouseX = sf::Mouse::getPosition(window).x;
		double mouseY = sf::Mouse::getPosition(window).y;

		float rotX = m_View->Sensitivity * (float)(mouseY - (m_View->Height / 2)) / m_View->Height;
		float rotY = m_View->Sensitivity * (float)(mouseX - (m_View->Width / 2)) / m_View->Width;

		glm::vec3 newOrientation = glm::rotate(m_View->Orientation, glm::radians(-rotX), glm::normalize(glm::cross(m_View->Orientation, m_View->Up)));

		if (abs(glm::angle(newOrientation, m_View->Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			m_View->Orientation = newOrientation;
		}

		m_View->Orientation = glm::rotate(m_View->Orientation, glm::radians(-rotY), m_View->Up);
		sf::Mouse::setPosition({ (m_View->Width / 2), (m_View->Height / 2) }, window);
	}
}

void Player::Update(sf::Window& window, float dt, bool noclip, Shader& shader)
{
	HandleMovement(window, dt, noclip);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		window.close();
	}

	m_BlockPosition.x = m_Position.x < 0 ? round((CHUNK_SIZE - 1 + ((int)m_Position.x % CHUNK_SIZE)) % CHUNK_SIZE) : (int)m_Position.x % CHUNK_SIZE;
	m_BlockPosition.y = m_Position.y < 0 ? round((CHUNK_SIZE - 1 + ((int)m_Position.y % CHUNK_SIZE)) % CHUNK_SIZE) : (int)m_Position.y % CHUNK_SIZE;
	m_BlockPosition.z = m_Position.z < 0 ? round((CHUNK_SIZE - 1 + ((int)m_Position.z % CHUNK_SIZE)) % CHUNK_SIZE) : (int)m_Position.z % CHUNK_SIZE;

	m_ChunkPosition.x = m_Position.x < 0 ? (m_Position.x - CHUNK_SIZE) / CHUNK_SIZE : m_Position.x / CHUNK_SIZE;
	m_ChunkPosition.y = m_Position.y < 0 ? (m_Position.y - CHUNK_SIZE) / CHUNK_SIZE : m_Position.y / CHUNK_SIZE;
	m_ChunkPosition.z = m_Position.z < 0 ? (m_Position.z - CHUNK_SIZE) / CHUNK_SIZE : m_Position.z / CHUNK_SIZE;

	m_View->UpdateMatrix(c_FOV, 0.2f, 1000.0f);
	m_View->ShaderMatrix(shader, "cameraMatrix");
}

void Player::Render(World& world)
{

}
