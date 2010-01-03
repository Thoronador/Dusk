/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2009-01-03
 Purpose: Weather Singleton class
          responsible for managing all weather effects in the game, e.g. fog,
          rain, snow

 History:
     - 2009-12-08 (rev 140) - initial version (by thoronador); fog handling
     - 2009-12-31 (rev 147) - documentation update
     - 2010-01-03 (rev 149) - snow implemented as particle system

 ToDo list:
     - other effects, such as rain, maybe even wind
     - find better parameters for snow particle system
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef WEATHER_H
#define WEATHER_H

#include <OgreColourValue.h>
#include <OgreLight.h>

namespace Dusk
{

  class Weather
  {
    public:
      ~Weather();

      /* Singleton access method */
      static Weather& getSingelton();

      /* sets the fog colour

         parameters:
             r, g, b - red, green and blue values of the fog colour
                       Each of these values has to be in [0;1].
      */
      void setFogColour(const float r, const float g, const float b);

      /* creates linear fog

         parameters:
             start_distance - distance (in world units) where the fog starts.
             max_distance   - distance at which fog becomes completely opaque.
      */
      void startLinearFog(const float start_distance, const float max_distance);

      /* creates exponential fog

         parameters:
             density - the density of the fog as a value between 0 and 1
      */
      void startExponentialFog(const float density);

      /* clears any fog, if present */
      void stopFog();

      /* returns true, if some kind of fog is present */
      bool isFoggy() const;

      /* returns true, if the currently active fog is linear fog */
      bool isLinearFog() const;

      /* let there be snow - and there will be snow ;) */
      void startSnow();

      /* removes any snow, if present */
      void stopSnow();

      /* returns true, if it currently snows */
      bool isSnowing() const;

    private:
      Weather();
      Weather(Weather& op) {}
      //fog-related
      float m_Fog_r, m_Fog_g, m_Fog_b;
      //render window's background colour before fog was applied
      Ogre::ColourValue m_orig_bgcolour;

      //name of the particle system for snow
      static const std::string cSnowParticleSystem;
  };//class

} //namespace

#endif // WEATHER_H
