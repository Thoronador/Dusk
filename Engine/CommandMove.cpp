#include "CommandMove.h"
#include <OgreVector3.h>
#include "Camera.h"
#include "Player.h"

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
        switch(m_Direction)
        {
            case FORWARD:
                Player::getSingleton().translate(Ogre::Vector3(0,0,-moveConstant));
                break;
            case BACKWARD:
                Player::getSingleton().translate(Ogre::Vector3(0,0,moveConstant));
                break;
            case LEFT:
                Player::getSingleton().translate(Ogre::Vector3(-moveConstant,0,0));
                break;
            case RIGHT:
                Player::getSingleton().translate(Ogre::Vector3(moveConstant,0,0));
                break;
            case TURN_LEFT:
                Camera::getSingleton().rotate(rotateDPS);
                break;
            case TURN_RIGHT:
                Camera::getSingleton().rotate(-rotateDPS);
                break;
            case JUMP_UP:
                Player::getSingleton().jump();
                break;
            default:
            break;
        }
        return true;
    }
}
