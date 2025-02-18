#include "CPowerups.h"

void CPowerups::SpawnPowerup(const sf::Vector2f& pos)
{
	auto powerup = std::make_unique<CPowerup>(m_gameWindow, m_paddleSprite, this);

	powerup->GetSprite()->setPosition(pos);
	m_powerups.push_back(std::move(powerup));
}

void CPowerups::RemovePowerups() const noexcept
{
	for (const auto& powerup : m_powerups)
	{
		powerup->Toggle(false);
		powerup->MarkForDeletion();
	}
}

CPowerup* CPowerups::GetActivePowerupByType(const ePowerupType& type, CPowerup* ignore) const noexcept
{
	for (const auto& powerup : m_powerups)
		if (powerup->GetType() == type && powerup.get() != ignore)
			if (powerup->IsActive() && !powerup->IsDrawable())
				return powerup.get();

	return nullptr;
}

std::vector<CPowerup*> CPowerups::GetActivePowerups() const
{
	std::vector<CPowerup*> activeList{};
	activeList.reserve(m_powerups.size());

	for (const auto& powerup : m_powerups)
		if (powerup->IsActive() && !powerup->IsDrawable() && !powerup->IsTimeless())
			activeList.push_back(powerup.get());

	return activeList;
}

bool CPowerups::DisablePowerupIfActive(const ePowerupType& type) const
{
	CPowerup* pwp = GetActivePowerupByType(type);
	if (!pwp)
		return false;

	pwp->Toggle(false);
	return true;
}

void CPowerups::Process(bool gamePaused)
{
	auto process = [=](const auto& powerup)
		{
			if (!powerup->IsActive())
				return true;

			powerup->Process(gamePaused);
			return false;
		};

	m_powerups.erase(std::remove_if(m_powerups.begin(), m_powerups.end(), process), m_powerups.end());

	if (m_isActiveGunPowerup && sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && !gamePaused)
		m_bullets->CreateBullet();
}
