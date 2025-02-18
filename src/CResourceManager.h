#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <filesystem>

class CResourceManager
{
public:
	CResourceManager();

	std::shared_ptr<sf::Sprite> GetSprite(const std::string& spriteName) const;
	std::shared_ptr<sf::Texture> GetTexture(const std::string& textureName) const;
	std::shared_ptr<sf::SoundBuffer> GetSoundBuffer(const std::string& soundName, bool canBeInvalid = false) const;

private:
	void CreateSprites();

	template <typename DataType>
	void LoadDataBuffer(const char* dirName, const char* fileExt, std::unordered_map<std::string, std::unique_ptr<DataType>>& dataArray) const
	{
		for (const auto& entry : std::filesystem::directory_iterator(dirName))
		{
			if (!entry.is_regular_file() || entry.path().extension() != fileExt)
				continue;

			auto buffer = std::make_unique<DataType>();
			if (!buffer->loadFromFile(entry.path().string()))
				throw std::runtime_error("Unable to load " + entry.path().filename().string());

			dataArray[entry.path().stem().string()] = std::move(buffer);
		}
	}

	template <typename DataType>
	void LoadDataBuffer(const char* dirName, const char* fileExt, std::unordered_map<std::string, std::shared_ptr<DataType>>& dataArray) const
	{
		for (const auto& entry : std::filesystem::directory_iterator(dirName))
		{
			if (!entry.is_regular_file() || entry.path().extension() != fileExt)
				continue;

			auto buffer = std::make_shared<DataType>();
			if (!buffer->loadFromFile(entry.path().string()))
				throw std::runtime_error("Unable to load " + entry.path().filename().string());

			dataArray[entry.path().stem().string()] = std::move(buffer);
		}
	}

private:
	std::unordered_map<std::string, std::shared_ptr<sf::Sprite>> m_spritesMap;
	std::unordered_map<std::string, std::shared_ptr<sf::Texture>> m_texturesMap;
	std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> m_soundsBuffer;
};

