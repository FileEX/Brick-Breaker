#pragma once
#include "CParticleEmitter.h"

class CParticleSystem
{
public:
	void AddEmitter(std::unique_ptr<CParticleEmitter> emitter) { m_emitters.push_back(std::move(emitter)); }
	void Process(float dt, bool gamePaused);

private:
	std::vector<std::unique_ptr<CParticleEmitter>> m_emitters;
};
