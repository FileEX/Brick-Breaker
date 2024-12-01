#pragma once
#include "SFML/Graphics.hpp"

class CParticle
{
public:
	CParticle(sf::RenderWindow* gameWindow, const sf::Vector2f& position, const sf::Vector2f& velocity, float life, float size, const sf::Color& color);

	bool IsAlive() const noexcept { return m_life > 0; }
	void Process(float dt, bool gamePaused);

private:
	sf::RenderWindow* m_gameWindow{ nullptr };

	sf::Vector2f m_velocity{};
	sf::Vector2f m_position{};

	sf::Color m_color;

	float m_size{ 0.0f };
	float m_life{ 0.0f };
	float m_initialLife{ 0.0f };

	sf::CircleShape m_particleShape;
};
