#include "CommandMove.h"
#include "Camera.h"
#include "DuskTypes.h"
namespace Dusk
{
    int CommandMove::moveConstant = 200;

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
            /*
            case DIRECTION::FORWARD:
                m_Camera->translate(Ogre::Vector3(0,moveConstant,0); // my be the wrong direction
                break;
            case DIRECTION::BACKWARD:
                m_Camera->translate(Ogre::Vector3(0,-moveConstant,0);
                break;
            case DIRECTION::LEFT:

                break;
            case DIRECTION::RIGHT:

                break;
            case DIRECTION::UP:

                break;
            case DIRECTION::DOWN:*/

                break;
            default:
            break;
        }
    }
}
