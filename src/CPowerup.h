#pragma once
#include "CResourceManager.h"

class CPowerups;

enum class ePowerupType
{
	UNKNOWN = -1,

	SMALL_PADDLE,
	WIDE_PADDLE,
	BONUS_LIFE,
	SLOW_BALL,
	FAST_BALL,
	MULTI_BALLS,
	GLUE,
	GUN,
	SKIP_LEVEL,

	MAX_POWERUPS,
};

class CPowerup
{
public:
	CPowerup(sf::RenderWindow* gameWindow, sf::Sprite* paddleSprite, CPowerups* powerups);

	const ePowerupType& GetType() const noexcept { return m_type; }
	sf::Sprite* GetSprite() const noexcept { return m_sprite.get(); }

	void MarkForDeletion() noexcept { m_isActive = false; }
	bool IsActive() const noexcept { return m_isActive; }
	bool IsDrawable() const noexcept { return m_draw; }

	void ResetLifeTime() { m_clock.restart(); }

	void Toggle(bool activate);
	void Process(bool gamePaused);

private:
	std::uint8_t GetRandomPowerupID() const noexcept;

private:
	sf::RenderWindow* m_gameWindow{ nullptr };
	CPowerups* m_powerups{ nullptr };
	sf::Sprite* m_paddleSprite{ nullptr };
	std::unique_ptr<sf::Sprite> m_sprite;

	ePowerupType m_type{ ePowerupType::UNKNOWN };

	bool m_isActive{ true };
	bool m_draw{ true };
	bool m_sound{ true };

	sf::Clock m_clock;

	int m_lifeTime{ 15 };
};
