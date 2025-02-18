#pragma once
#include "SFML/Graphics.hpp"

class CBrick
{
public:
	CBrick(sf::RenderWindow* gameWindow);

	void SetPosition(float x, float y) { m_sprite->setPosition(x, y); }
	void SetMaxHitsToDestroy(std::uint8_t maxHits) noexcept { m_maxHitToDestroy = maxHits; }
	void SetColor(std::uint8_t r, std::uint8_t g, std::uint8_t b) { m_sprite->setColor(sf::Color(r, g, b)); }

	sf::FloatRect GetGlobalBounds() const { return m_sprite->getGlobalBounds(); }
	sf::Sprite* GetSprite() const noexcept { return m_sprite.get(); }

	bool ShouldRemove() const noexcept { return m_markedForDeletion; }

	void Destroy(bool canSpawnPowerup = true);
	void Hit(std::uint8_t mult = 1);
	void Explode();

	void Process() const;

private:
	void MarkForDeletion() noexcept { m_markedForDeletion = true; }

private:
	sf::RenderWindow* m_gameWindow{ nullptr };
	std::unique_ptr<sf::Sprite> m_sprite;

	std::uint8_t m_hitCounter{ 0 };
	std::uint8_t m_maxHitToDestroy{ 4 };

	bool m_markedForDeletion{ false };
};
