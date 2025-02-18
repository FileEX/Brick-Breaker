#include "CParticleEmitter.h"
#include "Utils.h"

CParticleEmitter::CParticleEmitter(sf::RenderWindow* gameWindow, const sf::Vector2f& position, const EmitterData& data) : m_gameWindow(gameWindow)
{
	m_particles.reserve(data.count);

	for (std::size_t i = 0; i < data.count; i++)
	{
		float angle = (std::rand() % 360) * (M_PI / 180.0f);
		float speed = (std::rand() % 120) * 0.012f * 120.0f + 15.0f;
		speed *= data.speedMult;

		sf::Vector2f newDirection;
		newDirection.x = speed * std::cos(angle);

		if (data.randomDirection)
			newDirection.y = speed * std::sin(angle);
		else
			newDirection.y = std::fabs(data.direction.y * speed * std::sin(angle));

		float newSize = data.randomSize ? data.size * (std::rand() % 100) * 0.01f + 0.3f : data.size;
		float newLife = data.life * (std::rand() % 100) * 0.01f + 0.5f;
		
		sf::Color color = data.color;
		if (data.mixColor)
		{
			if (data.system == eParticleSystems::EXPLOSION)
				color = sf::Color(GetRandomNumber(220, 255), GetRandomNumber(50, 120), GetRandomNumber(0, 40));
		}

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

EmitterData CParticleEmitter::GetFXData(const eParticleSystems& particleSystem)
{
	EmitterData data;
	data.system = particleSystem;

	switch (particleSystem)
	{
		case eParticleSystems::BRICK:
		{
			data.randomDirection = false;
			data.direction = sf::Vector2f(0.0f, 1.0f);
			data.size = 3.0f;
			data.life = 2.0f;
			data.count = 40;

			break;
		}
		case eParticleSystems::EXPLOSION:
		{
			data.randomDirection = true;
			data.size = 5.0f;
			data.life = 3.0f;
			data.count = 100;
			data.speedMult = 2.0f;
			data.color = sf::Color(255, 115, 0);
			data.mixColor = true;

			break;
		}
	}

	return data;
}
