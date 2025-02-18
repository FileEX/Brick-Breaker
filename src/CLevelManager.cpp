#include "CLevelManager.h"
#include "CGame.h"

extern CGame* g_pGame;

CLevelManager::CLevelManager(sf::RenderWindow* gameWindow) : m_gameWindow(gameWindow)
{
	const sf::Font* font = g_pGame->GetHud()->GetFont();
	const sf::Vector2f& scale = g_pGame->GetHud()->GetScreenScale();
	const sf::Vector2f& gameSize = static_cast<sf::Vector2f>(m_gameWindow->getSize());

	m_bigText = sf::Text("0", *font, static_cast<std::uint32_t>(24 * scale.y));
	m_subText = sf::Text("0", *font, static_cast<std::uint32_t>(18 * scale.y));

	m_bigText.setPosition(0, gameSize.y / 2.0f - m_bigText.getGlobalBounds().height - 20 * scale.y);
	m_subText.setPosition(0, gameSize.y / 2.0f + m_subText.getGlobalBounds().height + 20 * scale.y);
}

void CLevelManager::SetLevelStatus(const eLevelStatus& status)
{
	m_status = status;

	if (m_status == eLevelStatus::PLAYING)
		g_pGame->Resume();
	else
	{
		if (m_status == eLevelStatus::FAIL)
		{
			m_bigText.setString("GAME OVER");
			m_subText.setString("Press SPACE to try again");

			g_pGame->GetAudio()->PlaySFX("game_over");
		}
		else if (m_status == eLevelStatus::SUCCESS)
		{
			m_bigText.setString("LEVEL COMPLETED");
			m_subText.setString("Press SPACE to continue");

			g_pGame->GetAudio()->PlaySFX("success");
		}

		g_pGame->Pause();
	}
}

void CLevelManager::LoadLevel()
{
	// TODO load level from save file
	m_level = 1;
	g_pGame->GetHud()->SetLevel(m_level);
}

void CLevelManager::ResetLevel()
{
	g_pGame->GetHud()->Reset(m_level);

	// Remove balls
	g_pGame->GetBalls()->RemoveBalls();
	g_pGame->GetBalls()->CreateBall();

	// Reset paddle scale
	g_pGame->GetPaddle()->UpdateScale(g_pGame->GetPaddle()->GetScale());

	g_pGame->GetBullets()->RemoveBullets();
	g_pGame->GetPowerups()->RemovePowerups();

	// Don't shoot the ball after disabling the glue powerup
	g_pGame->GetBalls()->GetFirstBall()->Restart();

	// Re-create bricks
	g_pGame->GetBricks()->RemoveBricks();
	g_pGame->GetBricks()->CreateBricks();

	g_pGame->Resume();
	m_status = eLevelStatus::PLAYING;
}

void CLevelManager::Process(bool gamePaused)
{
	if (!gamePaused)
		return;

	const sf::FloatRect& bigTextBounds = m_bigText.getGlobalBounds();
	const sf::FloatRect& subTextBounds = m_subText.getGlobalBounds();
	const sf::Vector2f& gameSize = static_cast<sf::Vector2f>(m_gameWindow->getSize());

	m_bigText.setPosition(gameSize.x / 2.0f - bigTextBounds.width / 2.0f, m_bigText.getPosition().y);
	m_subText.setPosition(gameSize.x / 2.0f - subTextBounds.width / 2.0f, m_subText.getPosition().y);

	m_gameWindow->draw(m_bigText);
	m_gameWindow->draw(m_subText);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		if (m_status == eLevelStatus::FAIL)
			ResetLevel();
		else if (m_status == eLevelStatus::SUCCESS)
		{
			m_level++;

			if (!std::filesystem::exists("data/level_" + std::to_string(m_level) + ".dat"))
				m_level = 1;

			ResetLevel();
		}
	}
}
