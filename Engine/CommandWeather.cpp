#include "CommandWeather.h"
#include "Weather.h"

namespace Dusk
{

CommandFog::CommandFog(const bool toggle, const bool turn_it_on)
{
  m_Toggle = toggle;
  m_TurnOn = turn_it_on;
}

CommandFog::~CommandFog()
{
  //empty
}

bool CommandFog::execute(Dusk::Scene* scene, int count)
{
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
  return true;
}

//snow
CommandSnow::CommandSnow(const bool toggle, const bool turn_it_on)
{
  m_Toggle = toggle;
  m_TurnOn = turn_it_on;
}

CommandSnow::~CommandSnow()
{
  //empty
}

bool CommandSnow::execute(Dusk::Scene* scene, int count)
{
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
  return true;
}

} //namespace
