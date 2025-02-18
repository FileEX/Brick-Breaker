#pragma once
#include "CBrick.h"
#include "CHud.h"

class CBricks
{
public:
	CBricks(sf::RenderWindow* gameWindow);

	void Shutdown() noexcept { m_bricksList.clear(); }

	void CreateBricks();
	void RemoveBricks();
	void DestroyBrick(CBrick* brick, bool reset = false);
	void DestroyAllBricks();

	std::size_t GetNumOfBricks() const noexcept { return m_bricksList.size(); }
	auto& GetBricks() const noexcept { return m_bricksList; }

	void Process() noexcept;

private:
	int GetHitsToDestroyFromColor(const sf::Color& color) const noexcept;
	void GenerateLevelFileFromTemplate() const;

private:
	sf::RenderWindow* m_gameWindow;

	std::vector<std::unique_ptr<CBrick>> m_bricksList;
};
