#pragma once
#include "CPowerup.h"
#include "CBullets.h"

class CPowerups
{
public:
	CPowerups(sf::RenderWindow* gameWindow, sf::Sprite* paddleSprite, CBullets* bullets) : m_gameWindow(gameWindow), m_paddleSprite(paddleSprite), m_bullets(bullets) {}

	void Shutdown() noexcept { m_powerups.clear(); }

	void SpawnPowerup(const sf::Vector2f& pos);
	void RemovePowerups() const noexcept;
	CPowerup* GetActivePowerupByType(const ePowerupType& type, CPowerup* ignore = nullptr) const noexcept;
	std::vector<CPowerup*> GetActivePowerups() const;

	void SetGunPowerupActive(bool active) noexcept { m_isActiveGunPowerup = active; }

	bool DisablePowerupIfActive(const ePowerupType& type) const;
	void Process(bool gamePaused);

private:
	sf::RenderWindow* m_gameWindow{ nullptr };
	sf::Sprite* m_paddleSprite{ nullptr };
	CBullets* m_bullets{ nullptr };

	bool m_isActiveGunPowerup{ false };

	std::vector<std::unique_ptr<CPowerup>> m_powerups;
};
