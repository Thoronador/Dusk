/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2008, 2009, 2010 walljumper, thoronador

    The Dusk Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Dusk Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Dusk Engine.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

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
