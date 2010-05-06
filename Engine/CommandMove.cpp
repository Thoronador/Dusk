#include "CommandMove.h"
#include "Camera.h"
#include <OgreVector3.h>

namespace Dusk
{
    int CommandMove::moveConstant = 20;
    const int CommandMove::rotateDPS = 72;

    CommandMove::CommandMove(const DIRECTION dir) : m_Direction(dir)
    {
        //ctor
    }

    CommandMove::~CommandMove()
    {
        //dtor
    }
    bool CommandMove::execute(Dusk::Scene* scene, int count)
    {
        Dusk::Camera& m_Camera = Camera::getSingleton();
        switch(m_Direction)
        {
            case FORWARD:
                m_Camera.translate(Ogre::Vector3(0,0,-moveConstant));
                break;
            case BACKWARD:
                m_Camera.translate(Ogre::Vector3(0,0,moveConstant));
                break;
            case LEFT:
                m_Camera.translate(Ogre::Vector3(-moveConstant,0,0));
                break;
            case RIGHT:
                m_Camera.translate(Ogre::Vector3(moveConstant,0,0));
                break;
            case TURN_LEFT:
                m_Camera.rotate(rotateDPS);
                break;
            case TURN_RIGHT:
                m_Camera.rotate(-rotateDPS);
                break;
            case JUMP_UP:
                m_Camera.jump();
                break;
            default:
            break;
        }
        return true;
    }
}
