#include "CParticleEmitter.h"
#include "Utils.h"

CParticleEmitter::CParticleEmitter(sf::RenderWindow* gameWindow, const sf::Vector2f& position, const sf::Vector2f& direction, const sf::Color& color, float size, float life, int count, bool randomSize) : m_gameWindow(gameWindow)
{
	for (std::size_t i = 0; i < count; i++)
	{
		float angle = (std::rand() % 360) * (M_PI / 180.0f);
		float speed = (std::rand() % 120) * 0.012f * 120.0f + 15.0f;

		sf::Vector2f newDirection;
		newDirection.x = speed * std::cos(angle);
		newDirection.y = std::fabs(direction.y * speed * std::sin(angle));

		float newSize = randomSize ? size * (std::rand() % 100) * 0.01f + 0.3f : size;
		float newLife = life * (std::rand() % 100) * 0.01f + 0.5f;
		
		m_particles.emplace_back(std::make_unique<CParticle>(gameWindow, position, newDirection, newLife, newSize, color));		
	}
}

void CParticleEmitter::Process(float dt, bool gamePaused)
{
	if (!m_active)
		return;

	auto process = [=](auto& particle)
		{
			if (!particle->IsAlive())
				return true;

			particle->Process(dt, gamePaused);
			return false;
		};

	m_particles.erase(std::remove_if(m_particles.begin(), m_particles.end(), process), m_particles.end());

	if (m_particles.empty())
		m_active = false;
}
