#include "CResourceManager.h"
#include "CGame.h"

extern CGame* g_pGame;

static const std::array<std::string, 3> staticTextures = {"paddle", "wall", "bg"};

CResourceManager::CResourceManager()
{
	try {
		LoadDataBuffer<sf::Texture>("textures", ".png", m_texturesMap);
		LoadDataBuffer<sf::SoundBuffer>("sfx", ".wav", m_soundsBuffer);
		LoadDataBuffer<sf::Texture>("textures/powerups", ".png", m_texturesMap);

		CreateSprites();
	}
	catch (std::runtime_error& e)
	{
		g_pGame->SetErrorStatus(e.what());
	}
}

void CResourceManager::CreateSprites()
{
	for (const auto& texture : m_texturesMap)
		if (std::find(staticTextures.begin(), staticTextures.end(), texture.first) != staticTextures.end() || texture.first.find("pwp_") != std::string::npos)
			m_spritesMap[texture.first] = std::make_shared<sf::Sprite>(*texture.second);
}

std::shared_ptr<sf::Sprite> CResourceManager::GetSprite(const std::string& spriteName) const
{
	if (m_spritesMap.find(spriteName) != m_spritesMap.end())
		return m_spritesMap.at(spriteName);
	else
	{
		g_pGame->SetErrorStatus("Sprite " + spriteName + " not found");
		return nullptr;
	}
}

std::shared_ptr<sf::Texture> CResourceManager::GetTexture(const std::string& textureName) const
{
	if (m_texturesMap.find(textureName) != m_texturesMap.end())
		return m_texturesMap.at(textureName);
	else
	{
		g_pGame->SetErrorStatus("Texture " + textureName + " not found");
		return nullptr;
	}
}

std::shared_ptr<sf::SoundBuffer> CResourceManager::GetSoundBuffer(const std::string& soundName, bool canBeInvalid) const
{
	if (m_soundsBuffer.find(soundName) != m_soundsBuffer.end())
		return m_soundsBuffer.at(soundName);
	else
	{
		if (!canBeInvalid)
			g_pGame->SetErrorStatus("Sound buffer " + soundName + " not found");

		return nullptr;
	}
}
