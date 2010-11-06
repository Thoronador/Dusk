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
 Purpose: Celestial class
          abstract class for all moving, celestial objects

 History:
     - 2010-08-22 (rev 233) - initial version (by thoronador)
     - 2010-08-23 (rev 234) - fixed an error in calculations

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.

 --------------------------------------------------------------------------*/

#ifndef CELESTIAL_H
#define CELESTIAL_H

namespace Dusk
{

class Celestial
{
  public:
    /* constructor */
    Celestial();

    /* destructor */
    virtual ~Celestial();

    /* sets the time (as hour of the day) when the object will appear. Values
       that are not within [0;24) will be ignored.

       parameters:
           s - time of appearance (e.g. sunrise)

       remarks:
           The time is interpreted as hour of the day in a 24-hour day.
           For example, 12.0 means noon, 6.5 is half past six in the morning,
           and 20.25 is quarter past eight in the evening.
    */
    void setStartTime(const float s);

    /* returns the time of the object's first appearance */
    float getStartTime() const;

    /* sets the time (as hour of the day) when the object will disappear. Values
       that are not within [0;24) will be ignored.

       remarks:
           For interpretation of time values see remarks of setStartTime().
    */
    void setEndTime(const float e);

    /* returns the time of the object's last appearance */
    float getEndTime() const;

    /* updates the object's time, allowing it to adjust its position

       remarks:
           For interpretation of time values see remarks of setStartTime().
    */
    virtual void updateTime(const float currentTime)=0;

    /* shows object */
    virtual void show()=0;

    /* hides object */
    virtual void hide()=0;

    /* returns true, if sun is currently visible */
    virtual bool isVisible() const=0;

  protected:
    /* returns the duration of object's visibility span in hours */
    float getSpan() const;

    /* returns true, if the given time is within the object's visibility span

       parameters:
           currentTime - the time

       remarks:
           For interpretation of time values see remarks of setStartTime().
    */
    bool isInSpan(const float currentTime) const;

    /* utility function that returns the part of time passed since the start

       parameters:
           currentTime - the time

       remarks:
           For interpretation of time values see remarks of setStartTime().
           The function will only return a reasonable result, if the given time
           is within the object's visibility span. Return values for times that
           are outside of this span are undefined.
    */
    float getSpanRatio(const float currentTime) const;

    //variables that hold the start and end times
    float m_Start, m_End;
};//class

} //namespace

#endif // CELESTIAL_H
