/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010 thoronador

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

/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-08-22
 Purpose: Sun class
          implements behaviour of sun in game

 History:
     - 2010-08-22 (rev 233) - initial version (by thoronador)
     - 2010-08-23 (rev 234) - changed implementation of movement
     - 2010-12-03 (rev 266) - use DuskLog/Messages class for logging

 ToDo list:
     - Find a better material for the sun - the current one displays the sun as
       a white rectangle when it's foggy.
     - Move sun further away from scene/ move it with player, because currently
       the sun sets right within a carwreck in the middle of the scene. :p
     - Implement colour-changing effect over time, i.e. something red in the
       morning/ evening and yellow at other times.
     - ???

 Bugs:
     - Sun is shown as white rectangle during fog.
     - Bugs? If you find one (or more), then tell me please.

 --------------------------------------------------------------------------*/

#ifndef SUN_H
#define SUN_H

#include "Celestial.h"
#include <OgreBillboardSet.h>
#include <OgreLight.h>

namespace Dusk
{

class Sun: public Celestial
{
  public:
    /* constructor */
    Sun();

    /* destructor */
    ~Sun();

    /* updates the object's time, allowing it to adjust its position

       remarks:
           The time is interpreted as hour of the day in a 24-hour day.
           For example, 12.0 means noon, 6.5 is half past six in the morning,
           and 20.25 is quarter past eight in the evening.
    */
    virtual void updateTime(const float currentTime);

    /* shows object */
    virtual void show();

    /* hides object */
    virtual void hide();

    /* returns true, if sun is currently visible */
    virtual bool isVisible() const;
  protected:
    Ogre::BillboardSet* m_BBSet;
    Ogre::Light* m_Light;
}; //class

} //namespace

#endif // SUN_H
