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
            case FORWARD:
                //m_Camera->translate(Ogre::Vector3(0,moveConstant,0)); // may be the wrong direction
                //I still get a linking error here, so I've put it into comments. :(
                break;
            case BACKWARD:
                //m_Camera->translate(Ogre::Vector3(0,-moveConstant,0));
                //I still get a linking error here, so I've put into comments. :(
                break;
            case LEFT:

                break;
            case RIGHT:

                break;
            case UP:

                break;
            case DOWN:

                break;
            default:
            break;
        }
        //we should do "return true;" or "return false;" here
    }
}
