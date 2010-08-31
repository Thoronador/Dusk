#include "ProjectileBase.h"
#include "DuskConstants.h"
#include <iostream>

namespace Dusk
{
ProjectileBase::ProjectileBase()
{
  //empty
}

ProjectileBase::~ProjectileBase()
{
  m_Projectiles.clear();
}

ProjectileBase& ProjectileBase::getSingleton()
{
  static ProjectileBase Instance;
  return Instance;
}

void ProjectileBase::clearAll()
{
  m_Projectiles.clear();
}

void ProjectileBase::addProjectile(const std::string& ID, const std::string& mesh,
                                   const float TTL, const float velocity,
                                   const uint8 n, const uint8 dice)
{
  ProjectileRecord temp;
  temp.Mesh = mesh;
  temp.DefaultTTL = TTL;
  temp.DefaultVelocity = velocity;
  temp.times = n;
  temp.dice = dice;
  addProjectile(ID, temp);
}

void ProjectileBase::addProjectile(const std::string& ID, const ProjectileRecord& data)
{
  if (ID!="" and data.Mesh!="")
  {
    m_Projectiles[ID] = data;
  }//if
}

bool ProjectileBase::hasProjectile(const std::string& ID) const
{
  return (m_Projectiles.find(ID) != m_Projectiles.end());
}

std::string ProjectileBase::getProjectileMesh(const std::string& ID, const bool UseMarkerOnError) const
{
  std::map<std::string, ProjectileRecord>::const_iterator iter;
  iter = m_Projectiles.find(ID);
  if (iter!=m_Projectiles.end())
  {
    return iter->second.Mesh;
  }
  if (UseMarkerOnError)
  {
    return cErrorMesh;
  }
  return "";
}

ProjectileRecord ProjectileBase::getProjectileData(const std::string& ID) const
{
  std::map<std::string, ProjectileRecord>::const_iterator iter;
  iter = m_Projectiles.find(ID);
  if (iter!=m_Projectiles.end())
  {
    return iter->second;
  }
  ProjectileRecord temp;
  temp.Mesh = "";
  temp.DefaultTTL = 0.00001;
  temp.DefaultVelocity = 1.0f;
  temp.dice = 0;
  temp.times = 0;
  return temp;
}

bool ProjectileBase::deleteProjectile(const std::string& ID)
{
  std::map<std::string, ProjectileRecord>::iterator iter;
  iter = m_Projectiles.find(ID);
  if (iter==m_Projectiles.end())
  {
    return false;
  }
  m_Projectiles.erase(iter);
  return true;
}

unsigned int ProjectileBase::getNumberOfProjectiles() const
{
  return m_Projectiles.size();
}

bool ProjectileBase::saveAllToStream(std::ofstream& OutStream) const
{
  if (!OutStream.good())
  {
    std::cout << "ProjectileBase::SaveAllToStream: ERROR: stream contains errors!\n";
    return false;
  }
  std::map<std::string, ProjectileRecord>::const_iterator iter;
  iter = m_Projectiles.begin();
  unsigned int len = 0;
  while (iter!=m_Projectiles.end())
  {
    //header "Proj"
    OutStream.write((char*) &cHeaderProj, sizeof(unsigned int));
    //ID
    len = iter->first.length();
    OutStream.write((char*) &len, sizeof(unsigned int));
    OutStream.write(iter->first.c_str(), len);
    //Mesh
    len = iter->second.Mesh.length();
    OutStream.write((char*) &len, sizeof(unsigned int));
    OutStream.write(iter->second.Mesh.c_str(), len);
    //TTL
    OutStream.write((char*) &(iter->second.DefaultTTL), sizeof(float));
    //velocity
    OutStream.write((char*) &(iter->second.DefaultVelocity), sizeof(float));
    //times
    OutStream.write((char*) &(iter->second.times), sizeof(uint8));
    //dice
    OutStream.write((char*) &(iter->second.dice), sizeof(uint8));
    ++iter;
  } //while
  return OutStream.good();
}

bool ProjectileBase::loadNextProjectileFromStream(std::ifstream& InStream)
{
  if (!InStream.good())
  {
    std::cout << "ProjectileBase::LoadNextProjectileFromStream: ERROR: stream contains errors!\n";
    return false;
  }
  unsigned int len = 0;
  InStream.read((char*) &len, sizeof(unsigned int));
  if (len != cHeaderProj)
  {
    std::cout << "ProjectileBase::LoadNextProjectileFromStream: ERROR: stream "
              << "contains unexpected header!\n";
    return false;
  }
  char Buffer[256];
  Buffer[0] = Buffer[255] = '\0';
  //read ID
  len = 0;
  InStream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    std::cout << "ProjectileBase::LoadNextProjectileFromStream: ERROR: ID is "
              << "longer than 255 characters!\n";
    return false;
  }
  InStream.read(Buffer, len);
  if (!InStream.good())
  {
    std::cout << "ProjectileBase::LoadNextProjectileFromStream: ERROR while "
              << "reading ID from stream!\n";
    return false;
  }
  Buffer[len] = '\0';
  const std::string projID = std::string(Buffer);

  //read Mesh
  len = 0;
  InStream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    std::cout << "ProjectileBase::LoadNextProjectileFromStream: ERROR: mesh "
              << "path is longer than 255 characters!\n";
    return false;
  }
  InStream.read(Buffer, len);
  if (!InStream.good())
  {
    std::cout << "ProjectileBase::LoadNextProjectileFromStream: ERROR while "
              << "reading mesh path from stream!\n";
    return false;
  }
  Buffer[len] = '\0';

  ProjectileRecord temp;
  temp.Mesh = std::string(Buffer);
  //TTL
  InStream.read((char*) &(temp.DefaultTTL), sizeof(float));
  //velocity
  InStream.read((char*) &(temp.DefaultVelocity), sizeof(float));
  //times
  InStream.read((char*) &(temp.times), sizeof(uint8));
  //dice
  InStream.read((char*) &(temp.dice), sizeof(uint8));
  if (!(InStream.good()))
  {
    std::cout << "ProjectileBase::LoadNextProjectileFromStream: ERROR while "
              << "reading projectile data from stream!\n";
    return false;
  }
  addProjectile(projID, temp);
  return true;
}

} //namespace
