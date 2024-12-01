#include "CBullets.h"

void CBullets::CreateBullet()
{
	if (m_lastShootTime.getElapsedTime().asMilliseconds() < 250)
		return;

	auto bullet_l = std::make_unique<CBullet>(m_gameWindow, m_bricks, true);
	m_bullets.push_back(std::move(bullet_l));

	auto bullet_r = std::make_unique<CBullet>(m_gameWindow, m_bricks, false);
	m_bullets.push_back(std::move(bullet_r));

	m_audio->PlaySFX("gun_shot");

	m_lastShootTime.restart();
}

void CBullets::RemoveBullets() const noexcept
{
	for (const auto& bullet : m_bullets)
		bullet->MarkForDeletion();
}

void CBullets::Process(float dt, bool gamePaused)
{
	auto process = [=](const auto& bullet)
		{
			if (bullet->ShouldRemove())
				return true;

			bullet->Process(dt, gamePaused);
			return false;
		};

	m_bullets.erase(std::remove_if(m_bullets.begin(), m_bullets.end(), process), m_bullets.end());
}
