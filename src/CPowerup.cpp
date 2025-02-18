#include "CPowerup.h"
#include "CGame.h"

static constexpr float MOVE_SPEED = 5.0f;
static constexpr int MIN_BRICKS_TO_SKIP_LEVEL = 8;
static constexpr float SKIP_LEVEL_CHANCE = 30.0f;

extern CGame* g_pGame;

CPowerup::CPowerup(sf::RenderWindow* gameWindow, sf::Sprite* paddleSprite, CPowerups* powerups) : m_gameWindow(gameWindow), m_paddleSprite(paddleSprite), m_powerups(powerups)
{
	std::uint8_t random = GetRandomPowerupID();
	m_type = static_cast<ePowerupType>(random);

	auto texture = g_pGame->GetResourceManager()->GetTexture("pwp_" + std::to_string(random));
	if (texture)
	{
		m_sprite = std::make_unique<sf::Sprite>(*texture);
		g_pGame->GetHud()->ScaleSprite(*m_sprite);
	}

	auto buffer = g_pGame->GetResourceManager()->GetSoundBuffer("pwp_" + std::to_string(random), true);
	if (!buffer)
		m_sound = false;

	m_clock.restart();
}

void CPowerup::Toggle(bool activate)
{
	CPowerup* pwp = m_powerups->GetPowerupByType(GetType(), this);
	if (pwp && pwp->IsActive() && !pwp->IsDrawable() && GetType() != ePowerupType::MULTI_BALLS)
	{
		pwp->ResetLifeTime();
		m_isActive = false;
		return;
	}

	switch (m_type)
	{
	case ePowerupType::SMALL_PADDLE:
	{
		const sf::Vector2f& paddleDefaultScale = g_pGame->GetPaddle()->GetScale();

		if (activate)
		{
			if (!m_powerups->DisablePowerupIfActive(ePowerupType::WIDE_PADDLE))
				g_pGame->GetPaddle()->UpdateScale(sf::Vector2f(paddleDefaultScale.x * 0.25f, paddleDefaultScale.y));
		}
		else
			g_pGame->GetPaddle()->UpdateScale(paddleDefaultScale);

		break;
	}
	case ePowerupType::WIDE_PADDLE:
	{
		const sf::Vector2f& paddleDefaultScale = g_pGame->GetPaddle()->GetScale();

		if (activate)
		{
			if (!m_powerups->DisablePowerupIfActive(ePowerupType::SMALL_PADDLE))
				g_pGame->GetPaddle()->UpdateScale(sf::Vector2f(paddleDefaultScale.x * 1.75f, paddleDefaultScale.y));
		}
		else
			g_pGame->GetPaddle()->UpdateScale(paddleDefaultScale);

		break;
	}
	case ePowerupType::BONUS_LIFE:
	{
		if (activate)
			g_pGame->GetHud()->AddLife();

		break;
	}
	case ePowerupType::SLOW_BALL:
	{
		if (activate)
			g_pGame->GetBalls()->SetBallsSpeedMult(0.5f);
		else
			g_pGame->GetBalls()->ResetBallsSpeed();

		break;
	}
	case ePowerupType::FAST_BALL:
	{
		if (activate)
			g_pGame->GetBalls()->SetBallsSpeedMult(1.5f);
		else
			g_pGame->GetBalls()->ResetBallsSpeed();

		break;
	}
	case ePowerupType::MULTI_BALLS:
	{
		if (activate)
			for (std::size_t i = 0; i < CBalls::MAX_BALLS; i++)
				g_pGame->GetBalls()->CreateBall(true);

		break;
	}
	case ePowerupType::GLUE:
	{
		g_pGame->GetPaddle()->SetGlued(activate);

		if (!activate)
			g_pGame->GetBalls()->ShootBall();

		break;
	}
	case ePowerupType::GUN:
	{
		g_pGame->GetPaddle()->SetGunActive(activate);
		g_pGame->GetPowerups()->SetGunPowerupActive(activate);
		break;
	}
	case ePowerupType::SKIP_LEVEL:
	{
		if (activate)
		{
			g_pGame->GetBricks()->DestroyAllBricks();
			g_pGame->GetLevelManager()->SetLevelStatus(eLevelStatus::SUCCESS);
		}

		break;
	}
	}
}

void CPowerup::Process(bool gamePaused)
{
	if (!m_isActive)
		return;

	if (m_clock.getElapsedTime().asSeconds() >= m_lifeTime && !gamePaused)
	{
		Toggle(false);
		m_isActive = false;
	}

	if (!m_draw)
		return;

	static const sf::Vector2u& windowSize = m_gameWindow->getSize();
	const sf::Vector2f& position = m_sprite->getPosition();
	if (position.y >= windowSize.y)
	{
		m_isActive = false;
		return;
	}

	if (!gamePaused)
		m_sprite->move(0, MOVE_SPEED);

	if (m_sprite->getGlobalBounds().intersects(m_paddleSprite->getGlobalBounds()))
	{
		Toggle(true);
		m_draw = false;

		if (m_sound)
			g_pGame->GetAudio()->PlaySFX("pwp_" + std::to_string(static_cast<std::uint8_t>(m_type)));
	}

	m_gameWindow->draw(*m_sprite);
}

std::uint8_t CPowerup::GetRandomPowerupID() const noexcept
{
	std::uint8_t random = std::rand() % static_cast<std::uint8_t>(ePowerupType::MAX_POWERUPS);
	bool skipLevel = static_cast<ePowerupType>(random) == ePowerupType::SKIP_LEVEL;

	if (g_pGame->GetBricks()->GetNumOfBricks() > MIN_BRICKS_TO_SKIP_LEVEL)
	{
		if (skipLevel)
			return GetRandomPowerupID();
	}
	else // SKIP_LEVEL_CHANCE % chance that the powerup will be a skip level
	{
		if (!skipLevel)
		{
			if (std::rand() % 100 < SKIP_LEVEL_CHANCE)
				return static_cast<std::uint8_t>(ePowerupType::SKIP_LEVEL);
		}
	}

	return random;
}
