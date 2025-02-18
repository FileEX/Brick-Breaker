#include "CBall.h"
#include "CGame.h"
#include "Utils.h"

extern CGame* g_pGame;

static constexpr float START_VELOCITY = 600.0f;
static constexpr float START_VELOCITY_X = 300.0f;
static constexpr float BALL_COL_PADDING = 5.0f;
static constexpr float TRAJECTORY_LINE_LEN = 400.0f;
static constexpr float TRAJECTORY_LINE_WIDTH = 3.0f;
static constexpr int MAX_TRAIL_POINTS = 10;

CBall::CBall(CPaddle* pPaddle, sf::RenderWindow* gameWindow, bool isAdditional) : m_paddle(pPaddle), m_gameWindow(gameWindow)
{
	m_bricks = g_pGame->GetBricks();
	m_sprite = std::make_unique<sf::Sprite>(*g_pGame->GetResourceManager()->GetTexture("ball"));
	g_pGame->GetHud()->ScaleSprite(*m_sprite, true);

	m_ballSize = m_sprite->getGlobalBounds().height;
	m_ballRadius = m_ballSize / 2.0f;
	m_gluedOffset = m_paddle->GetGlobalBounds().width / 2.0f - m_ballRadius;
	m_scale = m_sprite->getScale().y;

	const sf::Vector2f& scale = g_pGame->GetHud()->GetScreenScale();
	m_spriteStartSpeed = sf::Vector2f(START_VELOCITY_X * scale.x, START_VELOCITY * scale.y);

	if (isAdditional)
	{
		CBall* mainBall = g_pGame->GetBalls()->GetFirstBall();
		if (!mainBall)
		{
			MarkForDeletion();
			return;
		}

		m_shot = true;
		m_sprite->setPosition(mainBall->GetPosition());

		float angle = static_cast<float>(std::rand()) / RAND_MAX * 2.0f * M_PI;
		m_velocity = sf::Vector2f(std::cos(angle) * mainBall->GetSpeed(), std::rand() % 2 == 0 ? -START_VELOCITY * scale.y : START_VELOCITY * scale.y);
	}
}

void CBall::Shoot() noexcept
{
	if (m_shot)
		return;

	m_velocity.x = m_spriteStartSpeed.x * m_speedBostMult;
	m_velocity.y = -m_spriteStartSpeed.y * m_speedBostMult;
	m_shot = true;
	m_glued = false;
}

void CBall::Restart() noexcept
{
	m_shot = false;
	m_velocity.x = 0;
	m_velocity.y = 0;

	g_pGame->GetPowerups()->DisablePowerupIfActive(ePowerupType::SLOW_BALL);
	g_pGame->GetPowerups()->DisablePowerupIfActive(ePowerupType::FAST_BALL);

	m_trailPositions.clear();
}

bool CBall::KeepBallInScreenBounds(sf::Vector2f& ballPosition)
{
	static const sf::Vector2u& windowSize = m_gameWindow->getSize();
	static const sf::Vector3f& gameBounds = g_pGame->GetHud()->GetGameBounds();

	bool updatePosition = false;

	if (ballPosition.x - BALL_COL_PADDING <= gameBounds.x)
	{
		m_velocity.x = -m_velocity.x;
		ballPosition.x = gameBounds.x + BALL_COL_PADDING;

		updatePosition = true;
	}

	if (ballPosition.x + m_ballSize + BALL_COL_PADDING >= gameBounds.z)
	{
		m_velocity.x = -m_velocity.x;
		ballPosition.x = gameBounds.z - m_ballSize - BALL_COL_PADDING;

		updatePosition = true;
	}

	if (ballPosition.y - BALL_COL_PADDING <= gameBounds.y)
	{
		m_velocity.y = -m_velocity.y;
		ballPosition.y = gameBounds.y + BALL_COL_PADDING;

		updatePosition = true;
	}

	if (ballPosition.y + m_ballSize + BALL_COL_PADDING >= windowSize.y)
	{
		if (g_pGame->GetBalls()->GetNumberOfBalls() > 1)
			MarkForDeletion();
		else
		{
			g_pGame->GetHud()->TakeLife();

			if (g_pGame->GetHud()->GetLifes() > 0)
				Restart();
			else
				MarkForDeletion();
		}

		return false;
	}

	if (updatePosition)
	{
		m_sprite->setPosition(ballPosition);
		g_pGame->GetAudio()->PlaySFX("wall_hit");
	}

	return true;
}

void CBall::SetSpeedMult(float mult) noexcept
{
	m_speedBostMult = mult;
	m_velocity *= mult;
}

void CBall::ResetSpeed() noexcept
{
	m_velocity /= m_speedBostMult;
	m_speedBostMult = 1.0f;
}

void CBall::SetScale(float scale)
{
	m_sprite->setScale(m_scale * scale, m_scale * scale);

	m_ballSize = m_sprite->getGlobalBounds().height;
	m_ballRadius = m_ballSize / 2.0f;
	m_gluedOffset = m_paddle->GetGlobalBounds().width / 2.0f - m_ballRadius;
}

void CBall::Process(float dt, bool gamePaused)
{
	if (!m_shot && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !gamePaused)
		Shoot();

	if (!m_shot)
	{
		const sf::FloatRect& paddleBounds = m_paddle->GetGlobalBounds();
		if (!m_glued)
			m_gluedOffset = paddleBounds.width / 2.0f - m_ballRadius;

		m_sprite->setPosition(paddleBounds.left + m_gluedOffset, paddleBounds.top - m_ballSize - 0.5f);
	}
	else
	{
		if (!gamePaused)
			m_sprite->move(m_velocity * dt);

		ProcessTrailEffect();

		const sf::FloatRect& ballBounds = m_sprite->getGlobalBounds();
		sf::Vector2f ballPos = m_sprite->getPosition();

		if (!KeepBallInScreenBounds(ballPos))
			return;

		const sf::FloatRect& paddleBounds = m_paddle->GetGlobalBounds();
		if (ballBounds.intersects(paddleBounds))
		{
			if (m_paddle->IsGlued())
			{
				m_gluedOffset = ballPos.x - paddleBounds.left;
				m_glued = true;

				Restart();
				return;
			}

			float hitX = (ballPos.x - (paddleBounds.left + paddleBounds.width / 2)) / (paddleBounds.width / 2);
			m_velocity.x = hitX * m_spriteStartSpeed.x;
			m_velocity.y = -m_velocity.y;

			g_pGame->GetAudio()->PlaySFX("paddle_hit");

			ballPos.y = paddleBounds.top - m_ballSize - BALL_COL_PADDING;
			m_sprite->setPosition(ballPos);
		}

		for (const auto& brick : m_bricks->GetBricks())
		{
			const sf::FloatRect& brickBounds = brick->GetGlobalBounds();

			if (ballBounds.intersects(brickBounds))
			{
				float overlapX = std::min(ballPos.x + m_ballSize - brickBounds.left, brickBounds.left + brickBounds.width - ballPos.x);
				float overlapY = std::min(ballPos.y + m_ballSize - brickBounds.top, brickBounds.top + brickBounds.height - ballPos.y);

				if (overlapX < overlapY)
				{
					m_velocity.x = -m_velocity.x;

					if (ballPos.x <= brickBounds.left)
						ballPos.x = brickBounds.left - m_ballSize - BALL_COL_PADDING;
					else
						ballPos.x = brickBounds.left + brickBounds.width + m_ballSize + BALL_COL_PADDING;
				}
				else
				{
					m_velocity.y = -m_velocity.y;

					if (ballPos.y <= brickBounds.top)
						ballPos.y = brickBounds.top - m_ballSize - BALL_COL_PADDING;
					else
						ballPos.y = brickBounds.top + brickBounds.height + m_ballSize + BALL_COL_PADDING;
				}

				m_sprite->setPosition(ballPos);
				brick->Hit();
				break;
			}
		}
	}

	if (m_glued)
		DrawTrajectory();

	m_gameWindow->draw(*m_sprite);
}

void CBall::DrawTrajectory() const
{
	const sf::Vector2f direction(m_spriteStartSpeed.x, -m_spriteStartSpeed.y);
	const sf::Vector2f normalizedDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);;
	const sf::Vector2f startPoint = m_sprite->getPosition() + sf::Vector2f(m_ballRadius - TRAJECTORY_LINE_WIDTH * 0.5f, m_ballRadius);
	const sf::Vector2f& scale = g_pGame->GetHud()->GetScreenScale();

	sf::RectangleShape line(sf::Vector2f(TRAJECTORY_LINE_LEN * scale.y, TRAJECTORY_LINE_WIDTH * scale.x));
	line.setPosition(startPoint);
	line.setFillColor(sf::Color::White);
	line.setRotation(std::atan2(normalizedDirection.y, normalizedDirection.x) * 180.0f / M_PI);

	m_gameWindow->draw(line);
}

void CBall::ProcessTrailEffect()
{
	const sf::Vector2f& ballPos = m_sprite->getPosition();
	sf::Vector2f ballCenter = ballPos + sf::Vector2f(m_ballRadius, m_ballRadius);
	const sf::Vector2f& ballScale = m_sprite->getScale();

	m_trailPositions.push_back(ballCenter);
	if (m_trailPositions.size() > MAX_TRAIL_POINTS)
		m_trailPositions.pop_front();

	for (std::size_t i = 0; i < m_trailPositions.size(); ++i)
	{
		const float step = static_cast<float>(i) / m_trailPositions.size();

		const float alpha = std::lerp(0.0f, 200.0f, step);
		const float size = std::lerp((5.0f/1.0f) * ballScale.y, m_ballRadius, step);

		sf::CircleShape trail(size);
		trail.setOrigin(size, size);
		trail.setFillColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha)));
		trail.setPosition(m_trailPositions[i]);

		m_gameWindow->draw(trail);
	}
}
