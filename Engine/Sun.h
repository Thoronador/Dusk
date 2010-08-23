/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-08-22
 Purpose: Sun class
          implements behaviour of sun in game

 History:
     - 2010-08-22 (rev 233) - initial version (by thoronador)
     - 2010-08-23 (rev 234) - changed implementation of movement

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
