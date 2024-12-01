#include "CBullet.h"
#include "CGame.h"

extern CGame* g_pGame;

static constexpr float BULLET_SPEED = 8.0f;

CBullet::CBullet(sf::RenderWindow* gameWindow, CBricks* bricks, bool left) : m_gameWindow(gameWindow), m_bricks(bricks)
{
	m_sprite = sf::Sprite(*g_pGame->GetResourceManager()->GetTexture("bullet"));
	g_pGame->GetHud()->ScaleSprite(m_sprite);

	CPaddle* paddle = g_pGame->GetPaddle();

	const sf::FloatRect& bounds = m_sprite.getGlobalBounds();
	const sf::FloatRect& gunBounds = paddle->GetGunSprite()->getGlobalBounds();

	const sf::Vector2f& leftGunPos = paddle->GetGunSprite()->getPosition();
	const sf::Vector2f& rightGunPos = paddle->GetGunSprite(false)->getPosition();

	m_sprite.setPosition(left ? leftGunPos.x + gunBounds.width / 2.0f - bounds.width / 2.0f : rightGunPos.x + gunBounds.width / 2.0f - bounds.width / 2.0f, leftGunPos.y - bounds.height);
}

void CBullet::Process(float dt, bool gamePaused)
{
	static const sf::Vector3f& gameBounds = g_pGame->GetHud()->GetGameBounds();

	if (!m_canBeDeleted && m_sprite.getPosition().y <= gameBounds.y)
		m_canBeDeleted = true;

	if (!m_canBeDeleted)
	{
		for (const auto& brick : m_bricks->GetBricks())
		{
			if (m_sprite.getGlobalBounds().intersects(brick->GetGlobalBounds()))
			{
				brick->Hit();

				m_canBeDeleted = true;
				return;
			}
		}
	}

	if (!gamePaused)
		m_sprite.move(0, -BULLET_SPEED);

	m_gameWindow->draw(m_sprite);
}
