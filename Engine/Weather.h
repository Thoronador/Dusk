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
    private:
      Weather();
      Weather(Weather& op) {}
      //fog-related
      float m_Fog_r, m_Fog_g, m_Fog_b;
      Ogre::ColourValue m_orig_bgcolour;
  };//class

} //namespace

#endif // WEATHER_H
