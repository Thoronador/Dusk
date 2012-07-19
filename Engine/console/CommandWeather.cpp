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

#include "CommandWeather.h"
#include "Weather.h"

namespace Dusk
{

CommandWeather::CommandWeather(const WeatherType wt, const bool toggle, const bool turn_it_on)
{
  m_Toggle = toggle;
  m_TurnOn = turn_it_on;
  m_Type = wt;
}

CommandWeather::~CommandWeather()
{
  //empty
}

bool CommandWeather::execute(Dusk::Scene* scene, int count)
{
  switch(m_Type)
  {
    case wtFog:
         if (m_Toggle)
         {
           //switch from fog to no fog or vice versa
           if (Weather::getSingelton().isFoggy())
           {
             Weather::getSingelton().stopFog();
           }
           else
           {
             Weather::getSingelton().startLinearFog(50.0, 500.0);
           }
         }
         else
         {
           if (m_TurnOn)
           { //make it foggy
             Weather::getSingelton().startLinearFog(50.0, 500.0);
           }
           else
           { //no fog
             Weather::getSingelton().stopFog();
           }
         }
         break;
    case wtRain:
         if (m_Toggle)
         {
           //switch from rain to no rain or vice versa
           if (Weather::getSingelton().isRaining())
           {
             Weather::getSingelton().stopRain();
           }
           else
           {
             Weather::getSingelton().startRain();
           }
         }
         else
         {
           if (m_TurnOn)
           { //let it rain down
             Weather::getSingelton().startRain();
           }
           else
           { //no rain any more
             Weather::getSingelton().stopRain();
           }
         }
         break;
    case wtSnow:
         if (m_Toggle)
         {
           //switch from snow to no snow or vice versa
           if (Weather::getSingelton().isSnowing())
           {
             Weather::getSingelton().stopSnow();
           }
           else
           {
             Weather::getSingelton().startSnow();
           }
         }
         else
         {
           if (m_TurnOn)
           { //more snow
             Weather::getSingelton().startSnow();
           }
           else
           { //no more snow
             Weather::getSingelton().stopSnow();
           }
         }
         break;
  }//swi
  return true;
}

} //namespace
