#include "CBrick.h"
#include "CGame.h"
#include "Utils.h"

extern CGame* g_pGame;

static constexpr float POWERUP_CHANCE = 50.0f;
static int EXPLOSION_RADIUS = 0;

CBrick::CBrick(sf::RenderWindow* gameWindow) : m_gameWindow(gameWindow)
{
	m_sprite = std::make_unique<sf::Sprite>(*g_pGame->GetResourceManager()->GetTexture("brick"));

	m_sprite->setColor(sf::Color(
		static_cast<sf::Uint8>(std::rand() % 256),
		static_cast<sf::Uint8>(std::rand() % 256),
		static_cast<sf::Uint8>(std::rand() % 256)
	));
	
	g_pGame->GetHud()->ScaleSprite(*m_sprite);

	const sf::FloatRect& bounds = GetGlobalBounds();
	EXPLOSION_RADIUS = std::sqrt((bounds.width + 5) * (bounds.width + 5) + (bounds.height + 5) * (bounds.height + 5));
}

void CBrick::Destroy(bool canSpawnPowerup)
{
	EmitterData emitterData = CParticleEmitter::GetFXData(eParticleSystems::BRICK);
	emitterData.color = m_sprite->getColor();

	auto emitter = std::make_unique<CParticleEmitter>(m_gameWindow, m_sprite->getPosition(), emitterData);
	g_pGame->GetParticleSystem()->AddEmitter(std::move(emitter));

	if (canSpawnPowerup)
	{
		int spawnChance = m_maxHitToDestroy == 1 ? POWERUP_CHANCE / 2 : POWERUP_CHANCE;
		if (GetRandomNumber(0,99) < spawnChance)
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
		if (m_maxHitToDestroy == 255)
		{
			g_pGame->GetAudio()->PlaySFX("brick_hit_rubber");
			return;
		}

		g_pGame->GetAudio()->PlaySFX("brick_hit");

		//int textureIndex = std::min((m_hitCounter - 1) / (m_maxHitToDestroy / 3), 2);
		m_sprite->setTexture(*g_pGame->GetResourceManager()->GetTexture("brick_crack" + std::to_string(m_hitCounter)));
	}
}

void CBrick::Explode()
{
	const sf::FloatRect& targetBounds = GetGlobalBounds();
	float targetX = targetBounds.left;
	float targetY = targetBounds.top;

	float brickWidth = targetBounds.width;
	float brickHeight = targetBounds.height;
	float spacing = 5.0f;

	std::vector<CBrick*> neighbors;

	for (auto& brick : g_pGame->GetBricks()->GetBricks())
	{
		if (!brick || brick->ShouldRemove())
			continue;

		const sf::FloatRect& bounds = brick->GetGlobalBounds();
		float brickX = bounds.left;
		float brickY = bounds.top;

		bool isNeighbor =
			// Top
			((std::abs(brickY - (targetY - brickHeight)) <= spacing) && (std::abs(brickX - targetX) <= brickWidth + spacing)) ||
			// Bottom
			((std::abs(brickY - (targetY + brickHeight)) <= spacing) && (std::abs(brickX - targetX) <= brickWidth + spacing)) ||
			// Left
			((std::abs(brickX - (targetX - brickWidth)) <= spacing) && (std::abs(brickY - targetY) <= spacing)) ||
			// Right
			((std::abs(brickX - (targetX + brickWidth)) <= spacing) && (std::abs(brickY - targetY) <= spacing));

		if (isNeighbor)
			neighbors.push_back(brick.get());
	}

	if (!neighbors.empty())
	{		
		auto rng = std::default_random_engine{};
		std::ranges::shuffle(neighbors, rng);

		int countToDestroy = GetRandomNumber(1, std::min(3, (int)neighbors.size()));
		for (int i = 0; i < countToDestroy; ++i)
		{
			neighbors[i]->m_hitCounter = neighbors[i]->m_maxHitToDestroy;
			neighbors[i]->Hit();
		}
	}

	m_hitCounter = m_maxHitToDestroy;
	Hit();

	auto emitter = std::make_unique<CParticleEmitter>(g_pGame->GetGameWindow(), sf::Vector2f(targetX, targetY), CParticleEmitter::GetFXData(eParticleSystems::EXPLOSION));
	g_pGame->GetParticleSystem()->AddEmitter(std::move(emitter));

	g_pGame->GetAudio()->PlaySFX("explosion");
}

void CBrick::Process() const
{
	m_gameWindow->draw(*m_sprite);
}
