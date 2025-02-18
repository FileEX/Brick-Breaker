#pragma once
#include "CPaddle.h"
#include "CBricks.h"
#include <deque>

class CBall
{
public:
	CBall(CPaddle* pPaddle, sf::RenderWindow* gameWindow, bool isAdditional = false);

	const sf::Vector2f& GetPosition() const { return m_sprite->getPosition(); }
	float GetSpeed() const noexcept { return std::sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y); }

	void Shoot() noexcept;
	void Restart() noexcept;
	void MarkForDeletion() noexcept { m_markedForDeletion = true; }
	bool ShouldRemove() const noexcept { return m_markedForDeletion; }

	bool KeepBallInScreenBounds(sf::Vector2f& ballPosition);

	void SetSpeedMult(float mult) noexcept;
	void ResetSpeed() noexcept;

	void SetScale(float scale);

	void SetOnFire(bool onFire);

	void Process(float dt, bool gamePaused);

private:
	void DrawTrajectory() const;
	void ProcessTrailEffect();

private:
	sf::RenderWindow* m_gameWindow{ nullptr };
	CPaddle* m_paddle{ nullptr };
	CBricks* m_bricks{ nullptr };
	std::unique_ptr<sf::Sprite> m_sprite;

	sf::Vector2f m_velocity{};
	sf::Vector2f m_spriteStartSpeed{};

	sf::Color m_ballColor{ 255, 255, 255 };

	float m_ballSize{ 0.0f };
	float m_ballRadius{ 0.0f };
	float m_scale{ 1.0f };
	float m_speedBostMult{ 1.0f };
	float m_gluedOffset{ 0.0f };

	bool m_shot{ false };
	bool m_glued{ false };
	bool m_markedForDeletion{ false };
	bool m_isOnFire{ false };

	std::deque<sf::Vector2f> m_trailPositions;
};
