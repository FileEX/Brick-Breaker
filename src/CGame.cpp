#include "CGame.h"

CGame* g_pGame;

CGame::CGame()
{
	m_resourceManager = std::make_unique<CResourceManager>();
	g_pGame = this;
}

void CGame::Start()
{
	sf::RenderWindow window(sf::VideoMode(1600, 900), "Brick Breaker", sf::Style::Titlebar | sf::Style::Close);
	window.setMouseCursorVisible(false);
	window.setFramerateLimit(60);
	m_gameWindow = &window;

	m_clock = sf::Clock();
	std::srand(static_cast<std::uint32_t>(std::time(nullptr)));

	m_hud = std::make_unique<CHud>(m_gameWindow);
	m_paddle = std::make_unique<CPaddle>(m_resourceManager->GetSprite("paddle").get(), m_hud.get(), m_gameWindow);
	m_levelManager = std::make_unique<CLevelManager>(m_gameWindow);
	m_bricks = std::make_unique<CBricks>(m_gameWindow);
	m_balls = std::make_unique<CBalls>();
	m_particleSystem = std::make_unique<CParticleSystem>();
	m_audio = std::make_unique<CAudio>();
	m_bullets = std::make_unique<CBullets>(m_gameWindow, m_bricks.get(), m_audio.get());
	m_powerups = std::make_unique<CPowerups>(m_gameWindow, m_paddle->GetSprite(), m_bullets.get());

	// Create 'main' ball
	m_balls->CreateBall();

	m_bgSprite = m_resourceManager->GetSprite("bg").get();

	const sf::FloatRect& bounds = m_bgSprite->getGlobalBounds();
	m_bgSprite->setScale((1.0f / bounds.width) * m_gameWindow->getSize().x, (1.0f / bounds.height) * m_gameWindow->getSize().y);

	const sf::Font* arialFont = m_hud->GetFont();
	if (!arialFont)
		std::exit(EXIT_FAILURE);

	while (m_gameWindow->isOpen())
	{
		// Render error screen
		if (m_gameState == eGameState::ERROR_PAUSED)
		{
			m_gameWindow->clear();

			const sf::Vector2f& scale = m_hud->GetScreenScale();

			sf::Text header("Error", *arialFont, static_cast<std::uint32_t>(24 * scale.y));
			header.setStyle(sf::Text::Bold);

			const sf::Vector2u& windowSize = m_gameWindow->getSize();
			const sf::FloatRect& headerSize = header.getGlobalBounds();
			header.setPosition(sf::Vector2f(windowSize.x / 2 - headerSize.width / 2, windowSize.y / 2 - headerSize.height));

			sf::Text errorText(m_gameError, *arialFont, static_cast<std::uint32_t>(20 * scale.y));
			const sf::FloatRect& textSize = errorText.getGlobalBounds();
			errorText.setPosition(sf::Vector2f(windowSize.x / 2 - textSize.width / 2, windowSize.y / 2 + headerSize.height));

			m_gameWindow->draw(header);
			m_gameWindow->draw(errorText);
			m_gameWindow->display();
		}
		else
			Process(); // Process game logic

		static sf::Event event;
		while (m_gameWindow->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				m_audio->StopAll();
				m_gameWindow->close();
			}
		}
	}
}

void CGame::Process()
{
	if (!m_gameWindow->hasFocus())
		return;

	float dt = m_clock.restart().asSeconds();
	bool gamePaused = m_gameState == eGameState::PAUSED;

	m_gameWindow->clear();

	// Draw background
	m_gameWindow->draw(*m_bgSprite);

	// Draw paddle
	m_paddle->Process(gamePaused);

	// Draw bricks
	m_bricks->Process();

	// Draw balls
	m_balls->Process(dt, gamePaused);

	// Draw particles
	m_particleSystem->Process(dt, gamePaused);

	// Process sounds
	m_audio->Process();

	// Process & draw powerups
	m_powerups->Process(gamePaused);

	// Draw hud
	m_hud->Process();

	// Process bullets
	m_bullets->Process(dt, gamePaused);

	// Process level
	m_levelManager->Process(gamePaused);

	// Render game window
	m_gameWindow->display();
}

void CGame::SetErrorStatus(const std::string& error) noexcept
{
	m_gameState = eGameState::ERROR_PAUSED;
	m_gameError = error;
}
