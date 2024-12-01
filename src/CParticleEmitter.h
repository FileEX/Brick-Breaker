#pragma once
#include "CParticle.h"

class CParticleEmitter
{
public:
	CParticleEmitter(sf::RenderWindow* gameWindow, const sf::Vector2f& position, const sf::Vector2f& direction, const sf::Color& color, float size, float life, int count, bool randomSize = true);

	bool IsActive() const noexcept { return m_active; }
	void Process(float dt, bool gamePaused);

private:
	sf::RenderWindow* m_gameWindow{ nullptr };

	bool m_active{ true };

	std::vector<std::unique_ptr<CParticle>> m_particles;
};
