#include "CommandPlaySound.h"
#include "Sound.h"
namespace Dusk
{
    CommandPlaySound::CommandPlaySound(const std::string& path)
    {
        //ctor
        m_Path = path;
    }

    CommandPlaySound::~CommandPlaySound()
    {
        //dtor
    }

    bool CommandPlaySound::execute(Dusk::Scene* scene, int count)
    {
        Sound::get().Play(m_Path);
    }
}

