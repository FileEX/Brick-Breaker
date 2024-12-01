#include "CParticleSystem.h"

void CParticleSystem::Process(float dt, bool gamePaused)
{
    auto process = [=](auto& emitter)
        {
            if (!emitter->IsActive())
                return true;

            emitter->Process(dt, gamePaused);
            return false;
        };

    m_emitters.erase(std::remove_if(m_emitters.begin(), m_emitters.end(), process), m_emitters.end());
}
