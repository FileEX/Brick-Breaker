#include "CPowerup.h"
#include "CGame.h"

static constexpr float MOVE_SPEED = 5.0f;
static constexpr int MIN_BRICKS_TO_SKIP_LEVEL = 5;

static constexpr int SKIP_LEVEL_CHANCE = 22;
static constexpr int GUN_CHANCE = 25;
static constexpr int FIREBALL_CHANCE = 20;

extern CGame* g_pGame;

CPowerup::CPowerup(sf::RenderWindow* gameWindow, sf::Sprite* paddleSprite, CPowerups* powerups) : m_gameWindow(gameWindow), m_paddleSprite(paddleSprite), m_powerups(powerups)
{
	std::uint8_t random = GetRandomPowerupID();
	m_type = static_cast<ePowerupType>(random);
	m_lastPowerup = m_type;

	auto texture = g_pGame->GetResourceManager()->GetTexture("pwp_" + std::to_string(random));
	if (texture)
	{
		m_sprite = std::make_unique<sf::Sprite>(*texture);
		g_pGame->GetHud()->ScaleSprite(*m_sprite);
	}

	auto buffer = g_pGame->GetResourceManager()->GetSoundBuffer("pwp_" + std::to_string(random), true);
	if (!buffer)
		m_sound = false;

	m_timeText = std::make_unique<sf::Text>(std::to_string(m_lifeTime) + " s", *g_pGame->GetHud()->GetFont(), static_cast<std::uint32_t>(14 * g_pGame->GetHud()->GetScreenScale().y));

	m_clock.restart();
}

bool CPowerup::IsTimeless() const noexcept
{
	switch (GetType())
	{
		case ePowerupType::BONUS_LIFE:
		case ePowerupType::MULTI_BALLS:
		case ePowerupType::SKIP_LEVEL:
			return true;
	}

	return false;
}

void CPowerup::Toggle(bool activate)
{
	m_isActive = activate;

	if (activate)
	{
		CPowerup* pwp = m_powerups->GetActivePowerupByType(GetType(), this);
		if (pwp && !IsTimeless())
		{
			pwp->ResetLifeTime();
			m_isActive = false;
			return;
		}

		m_draw = false;
		m_clock.restart();
	}

	// Update powerups list
	g_pGame->GetHud()->Update();

	switch (GetType())
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
		case ePowerupType::FREEZE:
		{
			g_pGame->GetPaddle()->Freeze(activate);
			m_lifeTime = 4;

			break;
		}
		case ePowerupType::INVERTED_CONTROL:
		{
			g_pGame->GetPaddle()->Invert(activate);
			break;
		}
		case ePowerupType::FIRE_BALL:
		{
			g_pGame->GetBalls()->SetBallOnFire(activate);
			break;
		}
	}
}

void CPowerup::Process(bool gamePaused)
{
	if (!m_isActive)
		return;

	float elapsedTime = m_clock.getElapsedTime().asSeconds();
	if (elapsedTime >= m_lifeTime && !gamePaused && (!m_draw && m_isActive))
		Toggle(false);

	m_timeText->setString(std::to_string(static_cast<int>(std::floor(m_lifeTime - elapsedTime) + 1)) + " s");

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

		if (m_sound)
			g_pGame->GetAudio()->PlaySFX("pwp_" + std::to_string(static_cast<std::uint8_t>(m_type)));
	}

	m_gameWindow->draw(*m_sprite);
}

std::uint8_t CPowerup::GetRandomPowerupID() const noexcept
{
	std::uint8_t random = static_cast<std::uint8_t>(GetRandomNumber(0, static_cast<int>(ePowerupType::MAX_POWERUPS) - 1));
	
	ePowerupType pwpType = static_cast<ePowerupType>(random);
	bool skipLevel = pwpType == ePowerupType::SKIP_LEVEL;

	if (g_pGame->GetBricks()->GetNumOfBricks() > MIN_BRICKS_TO_SKIP_LEVEL)
	{
		if (skipLevel)
			return GetRandomPowerupID();
	}
	else // SKIP_LEVEL_CHANCE % chance that the powerup will be a skip level
	{
		int rand = GetRandomNumber(0, 99);

		if (!skipLevel)
			if (rand < SKIP_LEVEL_CHANCE)
				return static_cast<std::uint8_t>(ePowerupType::SKIP_LEVEL);
	}

	if ((pwpType == ePowerupType::BONUS_LIFE && g_pGame->GetHud()->GetLifes() == 3) || pwpType == m_lastPowerup || (pwpType == ePowerupType::SHIELD && m_powerups->GetActivePowerupByType(ePowerupType::SHIELD)) || (pwpType == ePowerupType::FIRE_BALL && m_powerups->GetActivePowerupByType(ePowerupType::FIRE_BALL)))
		return GetRandomPowerupID();

	int rand = GetRandomNumber(0, 99);

	if (pwpType == ePowerupType::GUN && rand >= GUN_CHANCE)
		return GetRandomPowerupID();

	if (pwpType == ePowerupType::FIRE_BALL && rand >= FIREBALL_CHANCE)
		return GetRandomPowerupID();

	return random;
}
