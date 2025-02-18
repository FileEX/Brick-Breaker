#pragma once
#include <SFML/Graphics.hpp>

enum class eLevelStatus
{
	PLAYING,
	SUCCESS,
	FAIL,
};

class CLevelManager
{
public:
	CLevelManager(sf::RenderWindow* gameWindow);

	std::uint8_t GetLevel() const noexcept { return m_level; }

	void SetLevelStatus(const eLevelStatus& status);
	eLevelStatus GetLevelStatus() const noexcept { return m_status; }

	void LoadLevel(std::uint8_t level);
	void ResetLevel();

	void PreProcess() const;
	void Process(bool gamePaused);

private:
	sf::RenderWindow* m_gameWindow{ nullptr };

	std::uint8_t m_level{ 1 };

	eLevelStatus m_status{ eLevelStatus::PLAYING };

	sf::Text m_bigText;
	sf::Text m_subText;

	sf::Sprite m_levelBackground;
	bool m_initalized{ false };
};

