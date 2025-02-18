#pragma once
#include "CBullet.h"
#include "CAudio.h"
#include "CBricks.h"

class CBullets
{
public:
	CBullets(sf::RenderWindow* gameWindow, CBricks* bricks, CAudio* audio) : m_gameWindow(gameWindow), m_bricks(bricks), m_audio(audio) {}

	void Shutdown() noexcept { m_bullets.clear(); }

	void CreateBullet();
	void RemoveBullets() const noexcept;

	void Process(float dt, bool gamePaused);

private:
	sf::RenderWindow* m_gameWindow{ nullptr };
	CAudio* m_audio{ nullptr };
	CBricks* m_bricks{ nullptr };

	sf::Clock m_lastShootTime;

	std::vector<std::unique_ptr<CBullet>> m_bullets;
};
