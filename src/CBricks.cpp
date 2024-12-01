#include "CBricks.h"
#include "CParticleSystem.h"
#include "CGame.h"
#include "fstream"

#define LEVELS_DEBUG_ACTIVE false

extern CGame* g_pGame;

CBricks::CBricks(sf::RenderWindow* gameWindow) : m_gameWindow(gameWindow)
{
	CreateBricks();

	if (LEVELS_DEBUG_ACTIVE)
		GenerateLevelFileFromTemplate();
}

void CBricks::CreateBricks()
{
	m_bricksList.clear();

	std::string path = "data/level_" + std::to_string(g_pGame->GetLevelManager()->GetLevel()) + ".dat";
	std::ifstream levelFile(path);
	if (!levelFile.is_open())
	{
		g_pGame->SetErrorStatus("Cannot load " + path + " file");
		return;
	}

	const SScreenSpace& bricksSpace = g_pGame->GetHud()->GetSpaceForBricks();
	const sf::Vector2f& scale = g_pGame->GetHud()->GetScreenScale();

	std::string line;
	while (std::getline(levelFile, line))
	{
		std::istringstream lineStream(line);

		float x, y;
		int hitsToDestroy;
		char separator;

		if (!(lineStream >> x >> separator >> y >> separator >> hitsToDestroy))
			continue;

		auto brick = std::make_unique<CBrick>(m_gameWindow);
		brick->SetPosition(bricksSpace.x + x * scale.x, bricksSpace.y + y * scale.y);

		m_bricksList.push_back(std::move(brick));
	}

	levelFile.close();
}

void CBricks::RemoveBricks()
{
	for (const auto& brick : m_bricksList)
		DestroyBrick(brick.get(), true);
}

void CBricks::DestroyBrick(CBrick* brick, bool reset)
{
	auto findBrick = [=](const auto& brickFromList)
		{
			return brickFromList.get() == brick;
		};

	m_bricksList.erase(std::remove_if(m_bricksList.begin(), m_bricksList.end(), findBrick), m_bricksList.end());

	if (m_bricksList.empty() && !reset)
		g_pGame->GetLevelManager()->SetLevelStatus(eLevelStatus::SUCCESS);
}

void CBricks::DestroyAllBricks()
{
	for (const auto& brick : m_bricksList)
		brick->Destroy(false);

	m_bricksList.clear();
}

void CBricks::Process() noexcept
{
	auto process = [](const auto& brick)
		{
			if (brick->ShouldRemove())
				return true;

			brick->Process();
			return false;
		};

	m_bricksList.erase(std::remove_if(m_bricksList.begin(), m_bricksList.end(), process), m_bricksList.end());
}

int CBricks::GetBrickHealthFromColor(const sf::Color& color) const
{
	return 4;
}

void CBricks::GenerateLevelFileFromTemplate() const
{
	sf::Image image;
	if (!image.loadFromFile("template.png"))
		return;

	const sf::Vector2f& imageSize = static_cast<sf::Vector2f>(image.getSize());
	const sf::Vector2f& scale = g_pGame->GetHud()->GetScreenScale();
	const sf::Vector2u& brickSize = g_pGame->GetResourceManager()->GetTexture("brick")->getSize();

	std::ofstream levelFile("level_gen.dat", std::ios::out | std::ios::trunc);
	if (!levelFile.is_open())
		return;

	for (std::uint32_t y = 0; y < imageSize.y; ++y)
	{
		for (std::uint32_t x = 0; x < imageSize.x; ++x)
		{
			const sf::Color& pixelColor = image.getPixel(x, y);
			if (pixelColor == sf::Color::Green)
			{
				levelFile << x * scale.x << ", " << y * scale.y << ", " << GetBrickHealthFromColor(image.getPixel(x + 2, y + 2)) << "\n";
				x += static_cast<std::uint32_t>(brickSize.x - 1);
			}
		}

		if (y % brickSize.y == 0)
			y += static_cast<std::uint32_t>(brickSize.y - 1);
	}

	levelFile.close();
}