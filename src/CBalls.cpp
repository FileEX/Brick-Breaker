#include "CBalls.h"
#include "CGame.h"

extern CGame* g_pGame;

void CBalls::CreateBall(bool additional)
{
	auto ball = std::make_unique<CBall>(g_pGame->GetPaddle(), g_pGame->GetGameWindow(), additional);
	m_balls.push_back(std::move(ball));
}

void CBalls::SetBallsSpeedMult(float mult) const noexcept
{
	for (const auto& ball : m_balls)
		ball->SetSpeedMult(mult);
}

void CBalls::ResetBallsSpeed() const noexcept
{
	for (const auto& ball : m_balls)
		ball->ResetSpeed();
}

void CBalls::ShootBall() const noexcept
{
	for (const auto& ball : m_balls)
		ball->Shoot();
}

void CBalls::SetBallScale(float scale) const
{
	for (const auto& ball : m_balls)
		ball->SetScale(scale);
}

void CBalls::SetBallOnFire(bool onFire) const
{
	for (std::size_t i = 0; i < MAX_FIRE_BALLS; i++)
		if (i < m_balls.size())
			m_balls[i]->SetOnFire(onFire);
}

void CBalls::RemoveBalls() const noexcept
{
	for (const auto& ball : m_balls)
		ball->MarkForDeletion();
}

void CBalls::Process(float dt, bool gamePaused)
{
	auto process = [=](const auto& ball)
		{
			if (ball->ShouldRemove())
				return true;

			ball->Process(dt, gamePaused);
			return false;
		};

	m_balls.erase(std::remove_if(m_balls.begin(), m_balls.end(), process), m_balls.end());
}
