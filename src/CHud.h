#pragma once
#include "Utils.h"
#include "SFML/Graphics.hpp"

struct SScreenSpace
{
	float x;
	float y;
	float width;
	float height;
};

class CHud
{
public:
	CHud(sf::RenderWindow* gameWindow);

	sf::Vector3f GetGameBounds() const;
	SScreenSpace GetSpaceForBricks() const;
	sf::Vector2f GetScreenScale() const;
	const sf::Font* GetFont() const noexcept { return m_fontLoaded ? &m_arialFont : nullptr; }
	std::uint8_t GetLifes() const noexcept { return m_lifes; }

	void ScaleSprite(sf::Sprite& sprite, bool keepRatio = false) const;

	void AddLife() noexcept { if (m_lifes < 3 && m_lifes > 0) m_lifes++; }
	void TakeLife();

	void AddScore(std::uint8_t score);
	void SetLevel(std::uint8_t level);

	void Reset(std::uint8_t level);

	void Process() const;

private:
	sf::RenderWindow* m_gameWindow{ nullptr };

	std::uint8_t m_lifes{ 3 };
	std::uint32_t m_score{ 0 };
	std::uint8_t m_level{ 1 };

	std::array<sf::Sprite, 3> m_heartsSprite{};
	std::array<sf::Sprite, 3> m_wallsSprite{};

	sf::Font m_arialFont;

	sf::Text m_levelText;
	sf::Text m_scoreText;

	bool m_fontLoaded{ false };
};
