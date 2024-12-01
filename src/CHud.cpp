#include "CHud.h"
#include "CGame.h"

extern CGame* g_pGame;

CHud::CHud(sf::RenderWindow* gameWindow) : m_gameWindow(gameWindow)
{
	const sf::Vector2f& windowSize = static_cast<sf::Vector2f>(m_gameWindow->getSize());

	// Create walls
	const sf::Texture& wallTexture = *g_pGame->GetResourceManager()->GetTexture("wall");
	float wallScale = 1.0f / wallTexture.getSize().y * windowSize.y;
	float wallsY = 60.0f * wallScale; // top offset

	for (std::uint8_t i = 0; i < 3; i++)
	{
		m_wallsSprite[i] = sf::Sprite(wallTexture);
		m_wallsSprite[i].setScale(1.0f, wallScale);

		if (i == 0) // left
			m_wallsSprite[i].setPosition(0.0f, wallsY);
		else if (i == 1) // right
		{
			m_wallsSprite[i].setRotation(180.0f);
			m_wallsSprite[i].setPosition(windowSize.x, wallsY + m_wallsSprite[i].getGlobalBounds().height);
		}
		else // top
		{
			m_wallsSprite[i].setRotation(90.0f);
			m_wallsSprite[i].setScale(1.0f, 1.0f / wallTexture.getSize().y * windowSize.x);
			m_wallsSprite[i].setPosition(windowSize.x, wallsY);
		}
	}

	// Create hearts
	for (std::uint8_t i = 0; i < m_heartsSprite.size(); i++)
	{
		const sf::Vector2f& gameScale = GetScreenScale();

		m_heartsSprite[i] = sf::Sprite(*g_pGame->GetResourceManager()->GetTexture("heart"));
		m_heartsSprite[i].setScale(gameScale.x * 0.5f, gameScale.y * 0.5f);

		const sf::FloatRect& bounds = m_heartsSprite[i].getGlobalBounds();
		m_heartsSprite[i].setPosition(30.0f * wallScale + (bounds.width + 20.0f * wallScale) * i, wallsY / 2.0f - bounds.height / 2.0f);
	}

	// Create score text
	m_fontLoaded = m_arialFont.loadFromFile("data/arial.ttf");
	if (!m_fontLoaded)
		return;

	m_scoreText = sf::Text("Score: 0", m_arialFont, static_cast<std::uint32_t>(24 * wallScale));
	const sf::FloatRect& scoreBounds = m_scoreText.getGlobalBounds();
	m_scoreText.setPosition(windowSize.x - scoreBounds.width - 30.0f * wallScale, wallsY / 2.0f - scoreBounds.height * 0.75f);

	// Create level text
	m_levelText = sf::Text("Level: 1", m_arialFont, static_cast<std::uint32_t>(24 * wallScale));
	const sf::FloatRect& levelBounds = m_levelText.getGlobalBounds();
	m_levelText.setPosition(windowSize.x / 2.0f - levelBounds.width / 2, m_scoreText.getPosition().y);
}

sf::Vector3f CHud::GetGameBounds() const
{
	float w = m_wallsSprite[0].getGlobalBounds().width;
	static const sf::Vector2u& windowSize = m_gameWindow->getSize();

	return sf::Vector3f(w, m_wallsSprite[2].getPosition().y + w, windowSize.x - w);
}

SScreenSpace CHud::GetSpaceForBricks() const
{
	const sf::Vector3f& bounds = GetGameBounds();
	SScreenSpace space{};
	space.x = bounds.x;
	space.y = bounds.y;
	space.width = bounds.z - bounds.x;
	space.height = m_gameWindow->getSize().y - bounds.y - 100.0f;

	return space;
}

sf::Vector2f CHud::GetScreenScale() const
{
	return sf::Vector2f((1.0f / 1600.0f) * m_gameWindow->getSize().x, (1.0f / 900.0f) * m_gameWindow->getSize().y);
}

void CHud::ScaleSprite(sf::Sprite& sprite, bool keepRatio) const
{
	if (!keepRatio)
		sprite.setScale(GetScreenScale());
	else
	{
		const sf::Vector2f& scale = GetScreenScale();
		sprite.setScale(scale.y, scale.y);
	}
}

void CHud::TakeLife()
{
	if (m_lifes > 0)
		m_lifes--;

	g_pGame->GetAudio()->PlaySFX("lost_ball");

	if (m_lifes <= 0)
	{
		g_pGame->GetLevelManager()->SetLevelStatus(eLevelStatus::FAIL);
		return;
	}
}

void CHud::AddScore(std::uint8_t score)
{
	m_score = m_score + score;
	m_scoreText.setString("Score: " + std::to_string(m_score));
}

void CHud::SetLevel(std::uint8_t level)
{
	m_level = level;
	m_levelText.setString("Level: " + std::to_string(m_level));
}

void CHud::Reset(std::uint8_t level)
{
	m_lifes = 3;
	m_score = 0;

	SetLevel(level);
	m_scoreText.setString("Score: " + std::to_string(m_score));
}

void CHud::Process() const
{
	// Draw lifes
	for (std::uint8_t i = 0; i < m_lifes; i++)
		m_gameWindow->draw(m_heartsSprite[i]);

	// Draw walls
	for (std::uint8_t i = 0; i < 3; i++)
		m_gameWindow->draw(m_wallsSprite[i]);

	// Draw score & level
	m_gameWindow->draw(m_scoreText);
	m_gameWindow->draw(m_levelText);
}
