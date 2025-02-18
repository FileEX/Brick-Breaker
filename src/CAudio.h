#pragma once
#include "CResourceManager.h"

class CAudio
{
public:
	void PlaySFX(const std::string& name);
	void PlayMusic();
	void ResetMusic(bool newLevel);

	void StopAll();

	void Process();

private:
	sf::Music m_music;

	std::vector<std::unique_ptr<sf::Sound>> m_playingSounds;
};
