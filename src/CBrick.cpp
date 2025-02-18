#include "CBrick.h"
#include "CGame.h"

extern CGame* g_pGame;

static constexpr float POWERUP_CHANCE = 50.0f;

CBrick::CBrick(sf::RenderWindow* gameWindow) : m_gameWindow(gameWindow)
{
	m_sprite = std::make_unique<sf::Sprite>(*g_pGame->GetResourceManager()->GetTexture("brick"));

	m_sprite->setColor(sf::Color(
		static_cast<sf::Uint8>(std::rand() % 256),
		static_cast<sf::Uint8>(std::rand() % 256),
		static_cast<sf::Uint8>(std::rand() % 256)
	));
	
	g_pGame->GetHud()->ScaleSprite(*m_sprite);
}

void CBrick::Destroy(bool canSpawnPowerup)
{
	auto emitter = std::make_unique<CParticleEmitter>(m_gameWindow, m_sprite->getPosition(), sf::Vector2f(0.0f, 1.0f), m_sprite->getColor(), 3.0f, 2.0f, 40);
	g_pGame->GetParticleSystem()->AddEmitter(std::move(emitter));

	if (canSpawnPowerup)
	{
		if (std::rand() % 100 < POWERUP_CHANCE)
			g_pGame->GetPowerups()->SpawnPowerup(m_sprite->getPosition());

		g_pGame->GetBricks()->DestroyBrick(this);
	}

	MarkForDeletion();
}

void CBrick::Hit(std::uint8_t mult)
{
	m_hitCounter = m_hitCounter + 1 * mult;
	if (m_hitCounter == 0)
		return;

	if (m_hitCounter >= m_maxHitToDestroy)
	{
		Destroy(true);
		g_pGame->GetAudio()->PlaySFX("brick_destroy");

		g_pGame->GetHud()->AddScore(10);
	}
	else
	{
		g_pGame->GetAudio()->PlaySFX("brick_hit");

		int textureIndex = std::min((m_hitCounter - 1) / (m_maxHitToDestroy / 3), 2);
		m_sprite->setTexture(*g_pGame->GetResourceManager()->GetTexture("brick_crack" + std::to_string(textureIndex + 1)));
	}
}

void CBrick::Process() const
{
	m_gameWindow->draw(*m_sprite);
}
