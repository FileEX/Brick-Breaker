#pragma once
#include "CBricks.h"

class CBullet
{
public:
	CBullet(sf::RenderWindow* gameWindow, CBricks* bricks, bool left);

	void MarkForDeletion() noexcept { m_canBeDeleted = true; }
	bool ShouldRemove() const noexcept { return m_canBeDeleted; }

	void Process(float dt, bool gamePaused);

private:
	sf::RenderWindow* m_gameWindow{ nullptr };
	CBricks* m_bricks{ nullptr };

	sf::Sprite m_sprite;

	bool m_canBeDeleted{ false };
};
