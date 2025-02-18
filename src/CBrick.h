#pragma once
#include "SFML/Graphics.hpp"

class CBrick
{
public:
	CBrick(sf::RenderWindow* gameWindow);

	void SetPosition(float x, float y) { m_sprite->setPosition(x, y); }

	sf::FloatRect GetGlobalBounds() const { return m_sprite->getGlobalBounds(); }
	bool ShouldRemove() const noexcept { return m_markedForDeletion; }

	void Destroy(bool canSpawnPowerup = true);
	void Hit(std::uint8_t mult = 1);
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
