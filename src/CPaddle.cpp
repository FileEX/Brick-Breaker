#include "CPaddle.h"
#include "CGame.h"

extern CGame* g_pGame;

static constexpr float CANNON_OFFSET_X = 6.0f;

CPaddle::CPaddle(sf::Sprite* paddleSprite, CHud* hud, sf::RenderWindow* gameWindow) : m_paddleSprite(paddleSprite), m_hud(hud), m_gameWindow(gameWindow)
{
	m_hud->ScaleSprite(*m_paddleSprite);

	const sf::FloatRect& paddleBounds = m_paddleSprite->getGlobalBounds();
	const float paddleY = gameWindow->getSize().y - paddleBounds.height - 20.0f;
	m_paddleSprite->setPosition(0.0f, paddleY);

	m_spriteScale = m_paddleSprite->getScale();

	const sf::Texture& texture = *g_pGame->GetResourceManager()->GetTexture("cannon");
	m_gunSprite_Left = sf::Sprite(texture);
	m_gunSprite_Right = sf::Sprite(texture);

	m_hud->ScaleSprite(m_gunSprite_Left);
	m_hud->ScaleSprite(m_gunSprite_Right);

	const sf::FloatRect& gunBounds = m_gunSprite_Left.getGlobalBounds();

	float gunY = paddleY - gunBounds.height * 0.75f;
	m_gunSprite_Left.setPosition(0.0f, gunY);
	m_gunSprite_Right.setPosition(0.0f, gunY);
}

void CPaddle::SetGlued(bool glued)
{
	m_paddleSprite->setTexture(*g_pGame->GetResourceManager()->GetTexture(glued ? "paddle_glued" : "paddle"));
	m_isGlued = glued;
}

void CPaddle::Process(bool gamePaused)
{
	static const sf::Vector3f& gameBounds = m_hud->GetGameBounds();
	static const sf::Vector2u& windowSize = m_gameWindow->getSize();
	const sf::FloatRect& bounds = m_paddleSprite->getGlobalBounds();

	if (!gamePaused)
		m_paddleSprite->setPosition(std::clamp<float>(sf::Mouse::getPosition(*m_gameWindow).x - bounds.width / 2, gameBounds.x, gameBounds.z - bounds.width), bounds.top);

	if (m_updateScale)
	{
		const float scaleX = m_paddleSprite->getScale().x;
		if (std::abs(scaleX - m_scaleTarget.x) > 0.01f)
		{
			const float sx = scaleX + (m_scaleTarget.x - scaleX) * 0.1f;
			m_paddleSprite->setScale(sx, m_spriteScale.y);

			m_gunSprite_Left.setScale(sx, m_spriteScale.y);
			m_gunSprite_Right.setScale(sx, m_spriteScale.y);
		}
		else
		{
			m_paddleSprite->setScale(m_scaleTarget.x, m_spriteScale.y);

			m_gunSprite_Left.setScale(m_scaleTarget.x, m_spriteScale.y);
			m_gunSprite_Right.setScale(m_scaleTarget.x, m_spriteScale.y);

			m_updateScale = false;
		}
	}

	m_gameWindow->draw(*m_paddleSprite);

	if (m_gunActive)
	{
		const sf::FloatRect& gunBounds = m_gunSprite_Left.getGlobalBounds();
		static const float gunPosY = m_gunSprite_Left.getPosition().y;
		const sf::FloatRect& bounds = m_paddleSprite->getGlobalBounds();

		m_gunSprite_Left.setPosition(bounds.left + CANNON_OFFSET_X * m_spriteScale.x, gunPosY);
		m_gunSprite_Right.setPosition(bounds.left + bounds.width - gunBounds.width - CANNON_OFFSET_X * m_spriteScale.x, gunPosY);

		m_gameWindow->draw(m_gunSprite_Left);
		m_gameWindow->draw(m_gunSprite_Right);
	}
}
