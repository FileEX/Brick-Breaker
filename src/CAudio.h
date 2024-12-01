#pragma once
#include "CResourceManager.h"

class CAudio
{
public:
	void PlaySFX(const std::string& name);
	void PlayMusic();

	void StopAll();

	void Process();

private:
	sf::Music m_music;
	std::uint8_t m_currentMusicID{ 0 };

	std::vector<std::unique_ptr<sf::Sound>> m_playingSounds;
};
