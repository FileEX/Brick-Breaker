#include "CParticle.h"

CParticle::CParticle(sf::RenderWindow* gameWindow, const sf::Vector2f& position, const sf::Vector2f& velocity, float life, float size, const sf::Color& color) : m_position(position), m_velocity(velocity), m_life(life), m_initialLife(life), m_size(size), m_color(color), m_gameWindow(gameWindow)
{
	m_particleShape = sf::CircleShape(m_size);
	m_particleShape.setFillColor(m_color);
	m_particleShape.setPosition(m_position);
}

void CParticle::Process(float dt, bool gamePaused)
{
	if (!gamePaused)
	{
		m_life -= dt;
		if (m_life <= 0.0f)
			return;

		m_particleShape.move(m_velocity * dt);

		m_color.a = static_cast<sf::Uint8>(255 * (m_life / m_initialLife));
		m_particleShape.setFillColor(m_color);
	}

	m_gameWindow->draw(m_particleShape);
}
