#include "CommandMove.h"
#include "Camera.h"
namespace Dusk
{
    int CommandMove::moveConstant = 200;

    CommandMove::CommandMove(CommandMove::Direction dir)
    {
        //ctor
        m_Direction = dir;
    }

    CommandMove::~CommandMove()
    {
        //dtor
    }
    bool CommandMove::execute(Dusk::Scene* scene, int count)
    {
        Dusk::Camera* m_Camera = getAPI().getDuskCamera();
    }
}
