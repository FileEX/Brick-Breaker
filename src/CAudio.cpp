#include "CAudio.h"
#include "CGame.h"

extern CGame* g_pGame;
static constexpr std::uint8_t numSongs = 6;

void CAudio::PlaySFX(const std::string& name)
{
	auto buffer = g_pGame->GetResourceManager()->GetSoundBuffer(name);
	if (!buffer)
		return;

	auto sfx = std::make_unique<sf::Sound>(*buffer);
	sfx->play();

	m_playingSounds.push_back(std::move(sfx));
}

void CAudio::PlayMusic()
{
	std::uint8_t random = (std::rand() % numSongs) + 1;
	if (m_currentMusicID > 0 && random == m_currentMusicID)
	{
		PlayMusic();
		return;
	}

	std::string path = "music/" + std::to_string(random) + ".ogg";
	if (!m_music.openFromFile(path))
	{
		g_pGame->SetErrorStatus("Cannot open " + path + " file");
		return;
	}

	m_currentMusicID = random;

	m_music.setVolume(50.0f);
	m_music.play();
}

void CAudio::StopAll()
{
	for (const auto& sfx : m_playingSounds)
		sfx->stop();

	m_music.stop();
	m_playingSounds.clear();
}

void CAudio::Process()
{
	auto process = [](const auto& sound)
		{
			return sound->getStatus() == sf::Sound::Stopped;
		};

	m_playingSounds.erase(std::remove_if(m_playingSounds.begin(), m_playingSounds.end(), process), m_playingSounds.end());

	if (m_music.getStatus() == sf::Music::Stopped)
		PlayMusic();
}
