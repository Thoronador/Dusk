#include "CommandMove.h"
#include "Camera.h"
#include "DuskTypes.h"
namespace Dusk
{
    int CommandMove::moveConstant = 20;
    const int CommandMove::rotateDPS = 72;

    CommandMove::CommandMove(DIRECTION dir) : m_Direction(dir)
    {
        //ctor
    }

    CommandMove::~CommandMove()
    {
        //dtor
    }
    bool CommandMove::execute(Dusk::Scene* scene, int count)
    {
        Dusk::Camera* m_Camera = getAPI().getDuskCamera();
        switch(m_Direction)
        {
            case FORWARD:
                m_Camera->translate(Ogre::Vector3(0,0,-moveConstant));
                //I still get a linking error here, so I've put it into comments. :(
                break;
            case BACKWARD:
                m_Camera->translate(Ogre::Vector3(0,0,moveConstant));
                //I still get a linking error here, so I've put into comments. :(
                break;
            case LEFT:
                m_Camera->translate(Ogre::Vector3(-moveConstant,0,0));
                break;
            case RIGHT:
                m_Camera->translate(Ogre::Vector3(moveConstant,0,0));
                break;
            case UP:
                m_Camera->translate(Ogre::Vector3(0,moveConstant,0));
                break;
            case DOWN:
                m_Camera->translate(Ogre::Vector3(0,-moveConstant,0));
                break;
            case TURN_LEFT:
                m_Camera->rotate(rotateDPS);
                break;
            case TURN_RIGHT:
                m_Camera->rotate(-rotateDPS);
                break;
            default:
            break;
        }
        //we should do "return true;" or "return false;" here
        return true;
    }
}
