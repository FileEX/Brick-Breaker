#pragma once
#include "CBall.h"

class CBalls
{
public:
	CBalls() { m_balls.reserve(MAX_BALLS + 1); }
	
	void CreateBall(bool additional = false);
	void Shutdown() noexcept { m_balls.clear(); }

	void SetBallsSpeedMult(float mult) const noexcept;
	void ResetBallsSpeed() const noexcept;
	void ShootBall() const noexcept;
	void SetBallScale(float scale) const;
	void SetBallOnFire(bool onFire) const;
	void RemoveBalls() const noexcept;

	CBall* GetFirstBall() const noexcept { return m_balls.empty() ? nullptr : m_balls.at(0).get(); }
	std::size_t GetNumberOfBalls() const noexcept { return m_balls.size(); }

	void Process(float dt, bool gamePaused);

public:
	static constexpr std::size_t MAX_BALLS = 5; // Maximum number of additional balls
	static constexpr std::size_t MAX_FIRE_BALLS = 2;

private:
	std::vector<std::unique_ptr<CBall>> m_balls;
};
