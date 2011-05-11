/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010, 2011 thoronador

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

#include "NPCBase.h"
#include "Messages.h"
#include "DuskConstants.h"

namespace Dusk
{

NPCAttributes NPCAttributes::getNullAttributes()
{
  NPCAttributes temp;
  temp.Str=temp.Agi=temp.Vit=temp.Int=temp.Will=temp.Cha=temp.Luck=0;
  return temp;
}

const NPCAnimations& NPCAnimations::getNullAnimations()
{
  static NPCAnimations anims;
  anims.Idle = anims.Walk = anims.MeleeAttack = anims.ProjectileAttack
             = anims.Jump = anims.Death = "";
  return anims;
}

const NPCTagPoints& NPCTagPoints::getNullTagPoints()
{
  static NPCTagPoints tags;
  tags.HandLeft = tags.HandRight = tags.SheathLeft = tags.SheathRight = "";
  return tags;
}

NPCBase::NPCBase()
{
  m_NPCList = std::map<std::string,NPCRecord>();
}

NPCBase::~NPCBase()
{
  m_NPCList.clear();
}

NPCBase& NPCBase::getSingleton()
{
  static NPCBase Instance;
  return Instance;
}

void NPCBase::addNPC(const std::string& ID, const std::string& Name,
                     const std::string& Mesh, const uint8 Level,
                     const NPCAttributes& Attr, const bool female,
                     const Inventory& StartingInventory,
                     const NPCAnimations& Anims, const NPCTagPoints& TagPoints)
{
  if (ID=="" or Name=="" or Mesh=="")
  {
    DuskLog() << "NPCBase::addNPC: ERROR: ID, name or mesh name is empty.\n";
    return;
  }
  std::map<std::string, NPCRecord>::iterator iter = m_NPCList.find(ID);
  if (iter!= m_NPCList.end())
  {
    iter->second.Name = Name;
    iter->second.Mesh = Mesh;
    iter->second.Level = Level;
    iter->second.Attributes = Attr;
    iter->second.Female = female;
    iter->second.InventoryAtStart.makeEmpty();
    StartingInventory.addAllItemsTo(iter->second.InventoryAtStart);
    iter->second.Animations = Anims;
    iter->second.TagPoints = TagPoints;
    return;
  }
  NPCRecord temp;
  temp.Name = Name;
  temp.Mesh = Mesh;
  temp.Level = Level;
  temp.Attributes = Attr;
  temp.Female = female;
  temp.InventoryAtStart = StartingInventory;
  temp.Animations = Anims;
  temp.TagPoints = TagPoints;
  m_NPCList[ID] = temp;
}

bool NPCBase::deleteNPC(const std::string& NPC_ID)
{
  std::map<std::string, NPCRecord>::iterator iter = m_NPCList.find(NPC_ID);
  if (iter==m_NPCList.end())
  {
    return false;
  }
  m_NPCList.erase(iter);
  return true;
}

bool NPCBase::hasNPC(const std::string& NPC_ID) const
{
  return (m_NPCList.find(NPC_ID) != m_NPCList.end());
}

void NPCBase::clearAllNPCs()
{
  m_NPCList.clear();
}

unsigned int NPCBase::getNumberOfNPCs() const
{
  return m_NPCList.size();
}

std::string NPCBase::getNPCName(const std::string& NPC_ID) const
{
  std::map<std::string, NPCRecord>::const_iterator iter = m_NPCList.find(NPC_ID);
  if (iter!=m_NPCList.end())
  {
    return iter->second.Name;
  }
  DuskLog() << "NPCBase::getNPCName: ERROR: no NPC with ID \""<<NPC_ID
            << "\" found. Returning empty string.\n";
  return "";
}

std::string NPCBase::getNPCMesh(const std::string& NPC_ID, const bool UseMarkerOnError) const
{
  std::map<std::string, NPCRecord>::const_iterator iter = m_NPCList.find(NPC_ID);
  if (iter!=m_NPCList.end())
  {
    return iter->second.Mesh;
  }
  //item with given ID is not present
  if (UseMarkerOnError)
  {
    DuskLog() << "NPCBase::getNPCMesh: ERROR: no NPC with ID \""<<NPC_ID
            << "\" found. Returning error marker's mesh instead.\n";
    return cErrorMesh;
  }
  DuskLog() << "NPCBase::getNPCMesh: ERROR: no NPC with ID \""<<NPC_ID
            << "\" found. Returning empty string.\n";
  return "";
}

uint8 NPCBase::getLevel(const std::string& NPC_ID) const
{
  std::map<std::string, NPCRecord>::const_iterator iter = m_NPCList.find(NPC_ID);
  if (iter!=m_NPCList.end())
  {
    return iter->second.Level;
  }
  DuskLog() << "NPCBase::getLevel: ERROR: no NPC with ID \""<<NPC_ID
            << "\" found. Returning zero.\n";
  return 0;
}

NPCAttributes NPCBase::getAttributes(const std::string& NPC_ID) const
{
  std::map<std::string, NPCRecord>::const_iterator iter = m_NPCList.find(NPC_ID);
  if (iter!=m_NPCList.end())
  {
    return iter->second.Attributes;
  }
  DuskLog() << "NPCBase::getAttributes: ERROR: no NPC with ID \""<<NPC_ID
            << "\" found. Returning zero attributes.\n";
  return NPCAttributes::getNullAttributes();
}

bool  NPCBase::isNPCFemale(const std::string& NPC_ID) const
{
  std::map<std::string, NPCRecord>::const_iterator iter = m_NPCList.find(NPC_ID);
  if (iter!=m_NPCList.end())
  {
    return iter->second.Female;
  }
  DuskLog() << "NPCBase::isNPCFemale: ERROR: no NPC with ID \""<<NPC_ID
            << "\" found. Returning false.\n";
  return false;
}

const Inventory& NPCBase::getNPCInventory(const std::string& NPC_ID) const
{
  std::map<std::string, NPCRecord>::const_iterator iter = m_NPCList.find(NPC_ID);
  if (iter!=m_NPCList.end())
  {
    return iter->second.InventoryAtStart;
  }
  DuskLog() << "NPCBase::getNPCInventory: ERROR: no NPC with ID \""<<NPC_ID
            << "\" found. Returning empty inventory.\n";
  return Inventory::getEmptyInventory();
}

const NPCAnimations& NPCBase::getNPCAnimations(const std::string& NPC_ID) const
{
  std::map<std::string, NPCRecord>::const_iterator iter = m_NPCList.find(NPC_ID);
  if (iter!=m_NPCList.end())
  {
    return iter->second.Animations;
  }
  DuskLog() << "NPCBase::getNPCAnimations: ERROR: no NPC with ID \""<<NPC_ID
            << "\" found. Returning null animations.\n";
  return NPCAnimations::getNullAnimations();
}

const NPCTagPoints& NPCBase::getNPCTagPoints(const std::string& NPC_ID) const
{
  std::map<std::string, NPCRecord>::const_iterator iter = m_NPCList.find(NPC_ID);
  if (iter!=m_NPCList.end())
  {
    return iter->second.TagPoints;
  }
  DuskLog() << "NPCBase::getNPCTagPoints: ERROR: no NPC with ID \""<<NPC_ID
            << "\" found. Returning empty tag points.\n";
  return NPCTagPoints::getNullTagPoints();
}

bool NPCBase::saveToStream(std::ofstream& output) const
{
  if (!(output.good()))
  {
    DuskLog() << "NPCBase::saveToStream: ERROR: Bad stream.\n";
    return false;
  }
  unsigned int len;
  std::map<std::string,NPCRecord>::const_iterator iter = m_NPCList.begin();
  while (iter != m_NPCList.end())
  {
    //header
    output.write((char*) &cHeaderNPC_, sizeof(unsigned int));
    //ID
    len = iter->first.length();
    output.write((char*) &len, sizeof(unsigned int));
    output.write(iter->first.c_str(), len);
    //NPC data
    // -- name
    len = iter->second.Name.length();
    output.write((char*) &len, sizeof(unsigned int));
    output.write(iter->second.Name.c_str(), len);
    // -- mesh
    len = iter->second.Mesh.length();
    output.write((char*) &len, sizeof(unsigned int));
    output.write(iter->second.Mesh.c_str(), len);
    // -- level
    output.write((char*) &(iter->second.Level), 1);
    // -- attributes
    output.write((char*) &(iter->second.Attributes.Str), 1);
    output.write((char*) &(iter->second.Attributes.Agi), 1);
    output.write((char*) &(iter->second.Attributes.Vit), 1);
    output.write((char*) &(iter->second.Attributes.Int), 1);
    output.write((char*) &(iter->second.Attributes.Will), 1);
    output.write((char*) &(iter->second.Attributes.Cha), 1);
    output.write((char*) &(iter->second.Attributes.Luck), 1);
    // -- female flag
    output.write((char*) &(iter->second.Female), sizeof(bool));
    //inventory
    if (!(iter->second.InventoryAtStart.saveToStream(output)))
    {
      DuskLog() << "NPCBase::saveToStream: ERROR while writing inventory data "
                << "of NPC \""<<iter->first<<"\" to stream.\n";
      return false;
    }
    // -- animations
    // ---- idle
    len = iter->second.Animations.Idle.length();
    output.write((char*) &len, sizeof(unsigned int));
    output.write(iter->second.Animations.Idle.c_str(), len);
    // ---- walk
    len = iter->second.Animations.Walk.length();
    output.write((char*) &len, sizeof(unsigned int));
    output.write(iter->second.Animations.Walk.c_str(), len);
    // ---- melee
    len = iter->second.Animations.MeleeAttack.length();
    output.write((char*) &len, sizeof(unsigned int));
    output.write(iter->second.Animations.MeleeAttack.c_str(), len);
    // ---- projectile
    len = iter->second.Animations.ProjectileAttack.length();
    output.write((char*) &len, sizeof(unsigned int));
    output.write(iter->second.Animations.ProjectileAttack.c_str(), len);
    // ---- jump
    len = iter->second.Animations.Jump.length();
    output.write((char*) &len, sizeof(unsigned int));
    output.write(iter->second.Animations.Jump.c_str(), len);
    // ---- death
    len = iter->second.Animations.Death.length();
    output.write((char*) &len, sizeof(unsigned int));
    output.write(iter->second.Animations.Death.c_str(), len);
    // -- tag points
    // ---- left hand
    len = iter->second.TagPoints.HandLeft.length();
    output.write((char*) &len, sizeof(unsigned int));
    output.write(iter->second.TagPoints.HandLeft.c_str(), len);
    // ---- right hand
    len = iter->second.TagPoints.HandRight.length();
    output.write((char*) &len, sizeof(unsigned int));
    output.write(iter->second.TagPoints.HandRight.c_str(), len);
    // ---- left sheath
    len = iter->second.TagPoints.SheathLeft.length();
    output.write((char*) &len, sizeof(unsigned int));
    output.write(iter->second.TagPoints.SheathLeft.c_str(), len);
    // ---- right sheath
    len = iter->second.TagPoints.SheathRight.length();
    output.write((char*) &len, sizeof(unsigned int));
    output.write(iter->second.TagPoints.SheathRight.c_str(), len);
    if (!output.good())
    {
      DuskLog() << "NPCBase::saveToStream: ERROR while writing animations and "
                << "tagpoints of NPC \""<<iter->first<<"\" to stream.\n";
      return false;
    }
    ++iter;
  } //while
  return output.good();
}

bool NPCBase::loadNextRecordFromStream(std::ifstream& input)
{
  if (!(input.good()))
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR: Bad stream.\n";
    return false;
  }
  unsigned int len = 0;
  //header
  input.read((char*) &len, sizeof(unsigned int));
  if (len != cHeaderNPC_)
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR: Invalid header.\n";
    return false;
  }
  //ID
  len = 0;
  input.read((char*) &len, sizeof(unsigned int));
  if (len > 255)
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR: NPC ID seems to be"
              << " longer than 255 characters.\n";
    return false;
  }
  char buffer[256];
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!(input.good()))
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR while reading NPC "
              << "ID.\n";
    return false;
  }
  const std::string tempID = std::string(buffer);
  //name
  len = 0;
  input.read((char*) &len, sizeof(unsigned int));
  if (len > 255)
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR: NPC name seems to "
              << "be longer than 255 characters.\n";
    return false;
  }
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!(input.good()))
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR while reading NPC "
              << "name from stream.\n";
    return false;
  }
  NPCRecord temp_rec;
  temp_rec.Name = std::string(buffer);
  //mesh
  len = 0;
  input.read((char*) &len, sizeof(unsigned int));
  if (len > 255)
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR: NPC mesh seems to "
              << "be longer than 255 characters.\n";
    return false;
  }
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!(input.good()))
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR while reading NPC "
              << "mesh location from stream.\n";
    return false;
  }
  temp_rec.Mesh = std::string(buffer);
  //level
  input.read((char*) &(temp_rec.Level), 1);
  //attributes
  input.read((char*) &(temp_rec.Attributes.Str), 1);
  input.read((char*) &(temp_rec.Attributes.Agi), 1);
  input.read((char*) &(temp_rec.Attributes.Vit), 1);
  input.read((char*) &(temp_rec.Attributes.Int), 1);
  input.read((char*) &(temp_rec.Attributes.Will), 1);
  input.read((char*) &(temp_rec.Attributes.Cha), 1);
  input.read((char*) &(temp_rec.Attributes.Luck), 1);
  //female?
  input.read((char*) &(temp_rec.Female), sizeof(bool));
  if (!(input.good()))
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR while reading NPC "
              << "level or attributes from stream.\n";
    return false;
  }
  //inventory
  if (!(temp_rec.InventoryAtStart.loadFromStream(input)))
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR while reading NPC "
              << "inventory from stream.\n";
    return false;
  }
  //animations
  // -- idle
  len = 0;
  input.read((char*) &len, sizeof(unsigned int));
  if (len > 255)
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR: NPC idle animation"
              << " name of \""<<tempID<<"\" seems to be longer than 255 "
              << "characters.\n";
    return false;
  }
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!(input.good()))
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR while reading NPC "
              << "idle animation name from stream.\n";
    return false;
  }
  temp_rec.Animations.Idle = std::string(buffer);
  // -- walk
  len = 0;
  input.read((char*) &len, sizeof(unsigned int));
  if (len > 255)
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR: NPC walk animation "
              << " name of \""<<tempID<<"\" seems to be longer than 255 "
              << "characters.\n";
    return false;
  }
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!(input.good()))
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR while reading NPC "
              << "walk animation name from stream.\n";
    return false;
  }
  temp_rec.Animations.Walk = std::string(buffer);
  // -- melee
  len = 0;
  input.read((char*) &len, sizeof(unsigned int));
  if (len > 255)
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR: NPC melee animation"
              << " name of \""<<tempID<<"\" seems to be longer than 255 "
              << "characters.\n";
    return false;
  }
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!(input.good()))
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR while reading NPC "
              << "melee animation name from stream.\n";
    return false;
  }
  temp_rec.Animations.MeleeAttack = std::string(buffer);
  // -- projectile
  len = 0;
  input.read((char*) &len, sizeof(unsigned int));
  if (len > 255)
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR: NPC projectile "
              << "animation name of\""<<tempID<< " seems to be longer than 255 "
              << "characters.\n";
    return false;
  }
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!(input.good()))
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR while reading NPC "
              << "projectile attack animation name from stream.\n";
    return false;
  }
  temp_rec.Animations.ProjectileAttack = std::string(buffer);
  // -- jump
  len = 0;
  input.read((char*) &len, sizeof(unsigned int));
  if (len > 255)
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR: NPC jum animation "
              << "name of \""<<tempID<<"\" seems to be longer than 255 "
              << "characters.\n";
    return false;
  }
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!(input.good()))
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR while reading NPC "
              << "jump animation name from stream.\n";
    return false;
  }
  temp_rec.Animations.Jump = std::string(buffer);
  // -- death
  len = 0;
  input.read((char*) &len, sizeof(unsigned int));
  if (len > 255)
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR: NPC death animation"
              << " name of \""<<tempID<<"\" seems to be longer than 255 "
              << "characters.\n";
    return false;
  }
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!(input.good()))
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR while reading NPC "
              << "death animation name from stream.\n";
    return false;
  }
  temp_rec.Animations.Death = std::string(buffer);

  //tag points
  // -- left hand
  len = 0;
  input.read((char*) &len, sizeof(unsigned int));
  if (len > 255)
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR: NPC TagPoint name"
              << " (left hand) of \""<<tempID<<"\" seems to be longer than 255"
              << " characters.\n";
    return false;
  }
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!(input.good()))
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR while reading NPC "
              << "TagPoint name (left hand) of \""<<tempID<<"\" from stream.\n";
    return false;
  }
  temp_rec.TagPoints.HandLeft = std::string(buffer);
  // -- right hand
  len = 0;
  input.read((char*) &len, sizeof(unsigned int));
  if (len > 255)
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR: NPC TagPoint name"
              << " seems to be longer than 255 characters.\n";
    return false;
  }
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!(input.good()))
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR while reading NPC "
              << "TagPoint name (right hand) of \""<<tempID<<"\" from stream.\n";
    return false;
  }
  temp_rec.TagPoints.HandRight = std::string(buffer);
  // -- left sheath
  len = 0;
  input.read((char*) &len, sizeof(unsigned int));
  if (len > 255)
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR: NPC TagPoint name"
              << " (left sheath) of \""<<tempID<<"\" seems to be longer than "
              << "255 characters.\n";
    return false;
  }
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!(input.good()))
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR while reading NPC "
              << "TagPoint name (left sheath) of \""<<tempID<<"\" from stream.\n";
    return false;
  }
  temp_rec.TagPoints.SheathLeft = std::string(buffer);
  // -- righ sheath
  len = 0;
  input.read((char*) &len, sizeof(unsigned int));
  if (len > 255)
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR: NPC TagPoint name"
              << " (left sheath) of \""<<tempID<<"\" seems to be longer than "
              << "255 characters.\n";
    return false;
  }
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!(input.good()))
  {
    DuskLog() << "NPCBase::loadNextRecordFromStream: ERROR while reading NPC "
              << "TagPoint name (right sheath) of \""<<tempID<<"\" from stream.\n";
    return false;
  }
  temp_rec.TagPoints.SheathRight = std::string(buffer);

  addNPC(tempID, temp_rec.Name, temp_rec.Mesh, temp_rec.Level,
         temp_rec.Attributes, temp_rec.Female, temp_rec.InventoryAtStart,
         temp_rec.Animations, temp_rec.TagPoints);
  return input.good();
}

#ifdef DUSK_EDITOR
NPCBase::Iterator NPCBase::getFirst() const
{
  return m_NPCList.begin();
}

NPCBase::Iterator NPCBase::getEnd() const
{
  return m_NPCList.end();
}
#endif

}//namespace
