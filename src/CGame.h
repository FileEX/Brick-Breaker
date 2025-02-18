#pragma once
#include "CResourceManager.h"
#include "CPaddle.h"
#include "CBall.h"
#include "CBalls.h"
#include "CBricks.h"
#include "CParticleSystem.h"
#include "CAudio.h"
#include "CPowerups.h"
#include "CHud.h"
#include "CBullets.h"
#include "CLevelManager.h"

enum class eGameState
{
	RUNNING,
	PAUSED,
	ERROR_PAUSED,
};

class CGame
{
public:
	CGame();
	void Start();

	void Pause() noexcept { m_gameState = eGameState::PAUSED; }
	void Resume() noexcept { m_gameState = eGameState::RUNNING; }

	void SetErrorStatus(const std::string& error) noexcept;

	sf::RenderWindow* GetGameWindow() const noexcept { return m_gameWindow; }
	CResourceManager* GetResourceManager() const noexcept { return m_resourceManager.get(); }

	CPaddle* GetPaddle() const noexcept { return m_paddle.get(); }
	CBalls* GetBalls() const noexcept { return m_balls.get(); }
	CBricks* GetBricks() const noexcept { return m_bricks.get(); }
	CParticleSystem* GetParticleSystem() const noexcept { return m_particleSystem.get(); }
	CAudio* GetAudio() const noexcept { return m_audio.get(); }
	CPowerups* GetPowerups() const noexcept { return m_powerups.get(); }
	CHud* GetHud() const noexcept { return m_hud.get(); }
	CBullets* GetBullets() const noexcept { return m_bullets.get(); }
	CLevelManager* GetLevelManager() const noexcept { return m_levelManager.get(); }

private:
	void Process();

private:
	sf::RenderWindow* m_gameWindow{ nullptr };
	sf::Sprite* m_bgSprite{ nullptr };
	std::unique_ptr<CResourceManager> m_resourceManager;
	std::unique_ptr<CPaddle> m_paddle;
	std::unique_ptr<CBalls> m_balls;
	std::unique_ptr<CBricks> m_bricks;
	std::unique_ptr<CParticleSystem> m_particleSystem;
	std::unique_ptr<CAudio> m_audio;
	std::unique_ptr<CPowerups> m_powerups;
	std::unique_ptr<CHud> m_hud;
	std::unique_ptr<CBullets> m_bullets;
	std::unique_ptr<CLevelManager> m_levelManager;

	sf::Clock m_clock;

	eGameState m_gameState{ eGameState::RUNNING };

	std::string m_gameError{};
};
