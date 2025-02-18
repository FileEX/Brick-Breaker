#pragma once
#include "CHud.h"

class CPaddle
{
public:
	CPaddle(sf::Sprite* paddleSprite, CHud* hud, sf::RenderWindow* gameWindow);

	const sf::FloatRect GetGlobalBounds() const { return m_paddleSprite->getGlobalBounds(); }
	sf::Sprite* GetSprite() const noexcept { return m_paddleSprite; }
	const sf::Sprite* GetGunSprite(bool left = true) const noexcept { return left ? &m_gunSprite_Left : &m_gunSprite_Right; }

	const sf::Vector2f GetScale() const noexcept { return m_spriteScale; }

	void SetGlued(bool glued);
	bool IsGlued() const noexcept { return m_isGlued; }

	void SetGunActive(bool active) noexcept { m_gunActive = active; }
	bool IsGunActive() const noexcept { return m_gunActive; }

	void UpdateScale(const sf::Vector2f& target) noexcept { m_scaleTarget = target; m_updateScale = true; }
	void Process(bool gamePaused);

private:
	sf::RenderWindow* m_gameWindow{ nullptr };
	CHud* m_hud{ nullptr };
	sf::Sprite* m_paddleSprite{ nullptr };

	sf::Sprite m_gunSprite_Left;
	sf::Sprite m_gunSprite_Right;

	sf::Vector2f m_scaleTarget{};
	sf::Vector2f m_spriteScale{};

	bool m_updateScale{ false };
	bool m_isGlued{ false };
	bool m_gunActive{ false };
};
