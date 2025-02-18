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
	
	m_frozenRect.setFillColor(sf::Color(157, 198, 245, 75));
}

void CPaddle::SetGlued(bool glued)
{
	m_paddleSprite->setTexture(*g_pGame->GetResourceManager()->GetTexture(glued ? "paddle_glued" : "paddle"));
	m_isGlued = glued;
}

void CPaddle::Freeze(bool freeze)
{
	m_frozen = freeze;

	if (freeze)
		UpdateFrozenRect();
}

void CPaddle::UpdateFrozenRect()
{
	const sf::FloatRect& paddleBounds = m_paddleSprite->getGlobalBounds();
	m_frozenRect.setScale(m_paddleSprite->getScale());

	float scale = (1.0f / 24.0f) * paddleBounds.height;
	m_frozenRect.setSize(sf::Vector2f(paddleBounds.width + 20.0f * scale, paddleBounds.height + 20.0f * scale));
	m_frozenRect.setPosition(paddleBounds.left - 10.0f * scale, paddleBounds.top - 10.0f * scale);
}

void CPaddle::Invert(bool invert)
{
	float paddleCenterX = m_paddleSprite->getPosition().x + m_paddleSprite->getGlobalBounds().width / 2.0f;

	if (invert)
	{
		m_offset = paddleCenterX;
		m_prevMouseX = static_cast<float>(sf::Mouse::getPosition(*m_gameWindow).x);;
	}
	else
		sf::Mouse::setPosition(sf::Vector2i(static_cast<int>(paddleCenterX), sf::Mouse::getPosition().y), *m_gameWindow);

	m_inverted = invert;
}

void CPaddle::Process(bool gamePaused)
{
	static const sf::Vector3f& gameBounds = m_hud->GetGameBounds();
	const sf::FloatRect& bounds = m_paddleSprite->getGlobalBounds();

	if (!gamePaused && !m_frozen)
	{
		float mouseX = sf::Mouse::getPosition(*m_gameWindow).x;

		if (m_inverted)
		{
			m_offset += -(mouseX - m_prevMouseX);
			m_prevMouseX = mouseX;

			float halfWidth = bounds.width * 0.5f;
			m_offset = std::clamp<float>(m_offset, gameBounds.x + halfWidth, gameBounds.z - halfWidth);
			
			m_paddleSprite->setPosition(m_offset - halfWidth, bounds.top);
		}
		else
			m_paddleSprite->setPosition(std::clamp<float>(mouseX - bounds.width / 2.0f, gameBounds.x, gameBounds.z - bounds.width), bounds.top);
	}

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

		if (m_frozen)
			UpdateFrozenRect();
	}

	m_gameWindow->draw(*m_paddleSprite);

	if (m_frozen)
		m_gameWindow->draw(m_frozenRect);

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
