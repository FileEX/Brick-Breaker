#pragma once
#include "CParticle.h"

enum class eParticleSystems
{
	BRICK,
	EXPLOSION,
};

struct EmitterData
{
	bool randomDirection{ true };
	sf::Vector2f direction{};
	bool mixColor{ false };
	sf::Color color{};
	bool randomSize{ true };
	float size{ 1.0f };
	float life{ 3.0f };
	int count{ 5 };
	float speedMult{ 1.0f };
	eParticleSystems system{};
};

class CParticleEmitter
{
public:
	CParticleEmitter(sf::RenderWindow* gameWindow, const sf::Vector2f& position, const EmitterData& data);

	bool IsActive() const noexcept { return m_active; }
	void Process(float dt, bool gamePaused);

	static EmitterData GetFXData(const eParticleSystems& particleSystem);

private:
	sf::RenderWindow* m_gameWindow{ nullptr };

	bool m_active{ true };

	std::vector<std::unique_ptr<CParticle>> m_particles;
};
