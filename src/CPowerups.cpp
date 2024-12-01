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

CPowerup* CPowerups::GetPowerupByType(const ePowerupType& type, CPowerup* ignore) const noexcept
{
	for (const auto& powerup : m_powerups)
		if (powerup->GetType() == type && powerup.get() != ignore)
			return powerup.get();

	return nullptr;
}

bool CPowerups::DisablePowerupIfActive(const ePowerupType& type) const
{
	CPowerup* pwp = GetPowerupByType(type);
	if (!pwp || !pwp->IsActive() || pwp->IsDrawable())
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
