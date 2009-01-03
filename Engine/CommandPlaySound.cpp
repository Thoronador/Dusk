#include "CommandPlaySound.h"
#include "Sound.h"
namespace Dusk
{
    CommandPlaySound::CommandPlaySound(const std::string& NoiseID)
    {
        //ctor
        m_Noise = NoiseID;
    }

    CommandPlaySound::~CommandPlaySound()
    {
        //dtor
    }

    bool CommandPlaySound::execute(Dusk::Scene* scene, int count)
    {
        return Sound::get().PlayNoise(m_Noise);
    }
}

